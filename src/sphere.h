#pragma once
#include "materials.h"
#include "math.h"
#include "rand.h"
#include "types.h"
#include <cstdlib>
#include <vector>

static std::vector<Sphere> spheres;
inline static void init_spheres() {
  spheres.reserve(488);
  spheres = {
      {{.center = {.x = -1.f, .y = 1.f, .z = -2.5f}, .mat = red_lambertian, .r = 1.f},
       {.center = {.x = 0.f, .y = 1.f, .z = 0.f}, .mat = glass, .r = 1.f},
       {.center = {.x = 1.f, .y = 1.f, .z = 2.5f}, .mat = copper_metallic, .r = 1.f},
       {.center = {.x = 0.f, .y = -1000.f, .z = 0.f}, .mat = silver_lambertian, .r = 1000.f}},
  };
  LCGRand lcg_rand;
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      float choose_mat = lcg_rand.rand_in_range(0, 1);
      Vec3 center = {
          .x = a + lcg_rand.rand_in_range(0, 1),
          .y = 0.2f,
          .z = b + 0.9f * lcg_rand.rand_in_range(0, 1),
      };
      if (choose_mat < 0.3) {
        // diffuse
        Color albedo = {
            .x = lcg_rand.rand_in_range(0, 1),
            .y = lcg_rand.rand_in_range(0, 1),
            .z = lcg_rand.rand_in_range(0, 1),
        };
        Material new_mat = {.atten = albedo, .type = MatType::lambertian};
        spheres.push_back(Sphere{.center = center, .mat = new_mat, .r = 0.2});
      } else if (choose_mat < 0.7) {
        // metal
        Color albedo = {
            .x = lcg_rand.rand_in_range(0.5, 1),
            .y = lcg_rand.rand_in_range(0.5, 1),
            .z = lcg_rand.rand_in_range(0.5, 1),
        };
        Material new_mat = {.atten = albedo, .type = MatType::metallic};
        spheres.push_back(Sphere{.center = center, .mat = new_mat, .r = 0.2});
      } else {
        // glass
        Material new_mat = {.atten = white, .type = MatType::dielectric};
        spheres.push_back(Sphere{.center = center, .mat = new_mat, .r = 0.2});
      }
    }
  }
}

#ifdef __ARM_NEON
#include <arm_neon.h>

// Returns hit t values or 0 depending on if this ray hit this sphere or not
[[nodiscard]] inline static float32x4_t sphere_hit(const RayCluster* rays, const Sphere* sphere,
                                              float t_max) {

  Vec3_128 sphere_center = broadcast_vec(&sphere->center);
  Vec3_128 oc = sphere_center - rays->orig;
  float rad_2 = sphere->r * sphere->r; //precomputable!!
  float32x4_t rad_2_vec = vdupq_n_f32(rad_2);

  float32x4_t a = dot(&rays->dir, &rays->dir);
  float32x4_t b = dot(&rays->dir, &oc);
  float32x4_t c = dot(&oc, &oc) - rad_2_vec;

  float32x4_t ac = vmulq_f32(a, c);
  float32x4_t nac = vnegq_f32(ac); //negate

  //inverted
  float32x4_t discrim = vmlaq_f32(nac, b, b);

  uint32x4_t hit_loc = vcgeq_f32(discrim, global::zeros);

  //hits? then > 0
  if (testz_128(hit_loc)) {
    return global::zeros;
  }

  // mask out the discriminants and b where there aren't hits
  //but with NEON is much better
  discrim = vbslq_f32(hit_loc, discrim, global::zeros);
  b = vbslq_f32(hit_loc, b, global::zeros);

  float32x4_t sqrt_d = vsqrtq_f32(discrim);
  float32x4_t recip_a = vrecpeq_f32(a);

  float32x4_t root = vmulq_f32(vsubq_f32(b, sqrt_d), recip_a);

  // allow through roots within the max t value
  float32x4_t t_max_vec = vdupq_n_f32(t_max);
  uint32x4_t below_max = vcltq_f32(root, t_max_vec);
  uint32x4_t above_min = vcgeq_f32(root, global::t_min_vec);
  hit_loc = vandq_u32(above_min, below_max);

  // Only clear materials can have another root thats worth finding.
  // This is why i only check for the farther out hit value if the material
  // is dielectric.
  if (testz_128(hit_loc) && sphere->mat.type == dielectric) {
    root = vmulq_f32(vaddq_f32(b, sqrt_d), recip_a);
    below_max = vcltq_f32(root, t_max_vec);
    above_min = vcgeq_f32(root, global::t_min_vec);
    hit_loc = vandq_u32(above_min, below_max);
  }

  root = vbslq_f32(hit_loc, root, global::zeros);
  return root;
}

inline static void set_face_normal(const RayCluster* rays, HitRecords* hit_rec,
                                   const Vec3_128* outward_norm) {
  float32x4_t ray_norm_dot = dot(&rays->dir, outward_norm);
  hit_rec->front_face = vcltq_f32(ray_norm_dot, global::zeros);
  hit_rec->norm = -*outward_norm;
  hit_rec->norm = blend_vec128(&hit_rec->norm, outward_norm, hit_rec->front_face);
}

inline static void create_hit_record(HitRecords* hit_rec, const RayCluster* rays,
                                     SphereCluster* sphere_cluster, float32x4_t t_vals) {
  hit_rec->t = t_vals;
  hit_rec->mat = sphere_cluster->mat;

  //inverted
  hit_rec->orig.x = vmlaq_f32(rays->orig.x, t_vals, rays->dir.x);
  hit_rec->orig.y = vmlaq_f32(rays->orig.y, t_vals, rays->dir.y);
  hit_rec->orig.z = vmlaq_f32(rays->orig.z, t_vals, rays->dir.z);

  Vec3_128 norm = hit_rec->orig - sphere_cluster->center;
  // normalize
  norm /= sphere_cluster->r;

  set_face_normal(rays, hit_rec, &norm);
}

// updates a sphere cluster with a sphere given a mask of where to insert the new sphere's values
inline static void update_sphere_cluster(SphereCluster* curr_cluster, Sphere curr_sphere,
                                         uint32x4_t update_mask) {

  if (testz_128(update_mask)) {
    return;
  }

  //NEON is much more direct
  Vec3_128 new_center = broadcast_vec(&curr_sphere.center);
  Vec3_128 new_atten = broadcast_vec(&curr_sphere.mat.atten);
  uint32x4_t new_type = vdupq_n_u32(curr_sphere.mat.type);
  float32x4_t new_r = vdupq_n_f32(curr_sphere.r);

  curr_cluster->center.x = vbslq_f32(update_mask, new_center.x, curr_cluster->center.x);
  curr_cluster->center.y = vbslq_f32(update_mask, new_center.y, curr_cluster->center.y);
  curr_cluster->center.z = vbslq_f32(update_mask, new_center.z, curr_cluster->center.z);

  curr_cluster->mat.atten.x = vbslq_f32(update_mask, new_atten.x, curr_cluster->mat.atten.x);
  curr_cluster->mat.atten.y = vbslq_f32(update_mask, new_atten.y, curr_cluster->mat.atten.y);
  curr_cluster->mat.atten.z = vbslq_f32(update_mask, new_atten.z, curr_cluster->mat.atten.z);

  curr_cluster->mat.type = vbslq_u32(update_mask, new_type, curr_cluster->mat.type);
  curr_cluster->r = vbslq_f32(update_mask, new_r, curr_cluster->r);
};

inline static void find_sphere_hits(HitRecords* hit_rec, const RayCluster* rays, float t_max) {

  SphereCluster closest_spheres = {
      .center =
          {
              .x = global::zeros,
              .y = global::zeros,
              .z = global::zeros,
          },
      .r = global::zeros,
  };

  float32x4_t max = vdupq_n_f32(global::float_max);

  // find first sphere as a baseline
  float32x4_t lowest_t_vals = sphere_hit(rays, &spheres[0], t_max);
  uint32x4_t hit_loc =  vmvnq_u32(vceqq_f32(lowest_t_vals, global::zeros));

  update_sphere_cluster(&closest_spheres, spheres[0], hit_loc);

  for (size_t i = 1; i < spheres.size(); i++) {
    float32x4_t new_t_vals = sphere_hit(rays, &spheres[i], t_max);

    // don't update on instances of no hits (hit locations all zeros)
    hit_loc = vmvnq_u32(vceqq_f32(new_t_vals, global::zeros));
    if (testz_128(hit_loc)) {
      continue;
    }

    uint32x4_t max_u = vreinterpretq_u32_f32(max);
    uint32x4_t new_t_vals_u = vreinterpretq_u32_f32(new_t_vals);

    // replace all 0's with float maximum to not replace actual values with
    // 0's during the minimum comparisons. Again, 0's represent no hits
    uint32x4_t no_hit_loc = veorq_u32(hit_loc, global::all_set);
    uint32x4_t max_mask = vandq_u32(no_hit_loc, max_u);
    new_t_vals = vreinterpretq_f32_u32(vorrq_u32(new_t_vals_u, max_mask));

    // replace 0's with max for current lowest too
    uint32x4_t curr_no_hit_loc = vceqq_f32(lowest_t_vals, global::zeros);
    max_mask = vandq_u32(curr_no_hit_loc, max_u);
    float32x4_t lowest_t_masked = vreinterpretq_f32_u32(vorrq_u32(vreinterpretq_u32_f32(lowest_t_vals), max_mask));

    // update sphere references based on where new
    // t values are closer than the current lowest
    uint32x4_t update_locs = vcltq_f32(new_t_vals, lowest_t_masked);
    update_sphere_cluster(&closest_spheres, spheres[i], update_locs);

    // update current lowest t values based on new t's, however, mask out
    // where we put float max values so that the t values still represent
    // no hits as 0.0
    lowest_t_vals = vminq_f32(lowest_t_masked, new_t_vals);
    uint32x4_t actual_vals_loc = vmvnq_u32(vceqq_f32(lowest_t_vals, max));
    lowest_t_vals = vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(lowest_t_vals), actual_vals_loc));
  }

  create_hit_record(hit_rec, rays, &closest_spheres, lowest_t_vals);
}

#else
#include <immintrin.h>

// Returns hit t values or 0 depending on if this ray hit this sphere or not
[[nodiscard]] inline static __m256 sphere_hit(const RayCluster* rays, const Sphere* sphere,
                                              float t_max) {

  Vec3_256 sphere_center = broadcast_vec(&sphere->center);
  Vec3_256 oc = sphere_center - rays->orig;
  float rad_2 = sphere->r * sphere->r;
  __m256 rad_2_vec = _mm256_broadcast_ss(&rad_2);

  __m256 a = dot(&rays->dir, &rays->dir);
  __m256 b = dot(&rays->dir, &oc);
  __m256 c = dot(&oc, &oc) - rad_2_vec;

  __m256 discrim = _mm256_fmsub_ps(b, b, a * c);

  __m256 hit_loc = _mm256_cmp_ps(discrim, global::zeros, global::cmpnlt);
  int no_hit = _mm256_testz_ps(hit_loc, hit_loc);

  if (no_hit) {
    return global::zeros;
  }

  // mask out the discriminants and b where there aren't hits
  discrim = _mm256_and_ps(discrim, hit_loc);
  b = _mm256_and_ps(b, hit_loc);

  __m256 sqrt_d = _mm256_sqrt_ps(discrim);
  __m256 recip_a = _mm256_rcp_ps(a);

  __m256 root = (b - sqrt_d) * recip_a;

  // allow through roots within the max t value
  __m256 t_max_vec = _mm256_broadcast_ss(&t_max);
  __m256 below_max = _mm256_cmp_ps(root, t_max_vec, global::cmplt);
  __m256 above_min = _mm256_cmp_ps(root, global::t_min_vec, global::cmpnlt);
  hit_loc = _mm256_and_ps(above_min, below_max);

  // Only clear materials can have another root thats worth finding.
  // This is why i only check for the farther out hit value if the material
  // is dielectric.
  if (_mm256_testz_ps(hit_loc, hit_loc) && sphere->mat.type == dielectric) {
    root = (b + sqrt_d) * recip_a;
    below_max = _mm256_cmp_ps(root, t_max_vec, global::cmplt);
    above_min = _mm256_cmp_ps(root, global::t_min_vec, global::cmpnlt);
    hit_loc = _mm256_and_ps(above_min, below_max);
  }
  root = _mm256_and_ps(root, hit_loc);

  return root;
}

inline static void set_face_normal(const RayCluster* rays, HitRecords* hit_rec,
                                   const Vec3_256* outward_norm) {
  __m256 ray_norm_dot = dot(&rays->dir, outward_norm);
  hit_rec->front_face = _mm256_cmp_ps(ray_norm_dot, _mm256_setzero_ps(), global::cmplt);
  hit_rec->norm = -*outward_norm;
  hit_rec->norm = blend_vec256(&hit_rec->norm, outward_norm, hit_rec->front_face);
}

inline static void create_hit_record(HitRecords* hit_rec, const RayCluster* rays,
                                     SphereCluster* sphere_cluster, __m256 t_vals) {
  hit_rec->t = t_vals;
  hit_rec->mat = sphere_cluster->mat;

  hit_rec->orig.x = _mm256_fmadd_ps(rays->dir.x, t_vals, rays->orig.x);
  hit_rec->orig.y = _mm256_fmadd_ps(rays->dir.y, t_vals, rays->orig.y);
  hit_rec->orig.z = _mm256_fmadd_ps(rays->dir.z, t_vals, rays->orig.z);

  Vec3_256 norm = hit_rec->orig - sphere_cluster->center;
  // normalize
  norm /= sphere_cluster->r;

  set_face_normal(rays, hit_rec, &norm);
}

// updates a sphere cluster with a sphere given a mask of where to insert the new sphere's values
inline static void update_sphere_cluster(SphereCluster* curr_cluster, Sphere curr_sphere,
                                         __m256 update_mask) {

  if (_mm256_testz_ps(update_mask, update_mask)) {
    return;
  }

  SphereCluster new_spheres = {
      .center = broadcast_vec(&curr_sphere.center),
      .mat =
          {
              .atten = broadcast_vec(&curr_sphere.mat.atten),
              .type = _mm256_set1_epi32(curr_sphere.mat.type),
          },
      .r = _mm256_broadcast_ss(&curr_sphere.r),

  };

  new_spheres.center &= update_mask;
  new_spheres.mat.atten &= update_mask;
  new_spheres.mat.type = _mm256_and_si256(new_spheres.mat.type, (__m256i)update_mask);
  new_spheres.r = _mm256_and_ps(new_spheres.r, update_mask);

  // negation of update locations so we can preserve current values
  // while clearing bits where we will update
  __m256 preserve_curr = _mm256_xor_ps(update_mask, global::all_set);

  SphereCluster curr_spheres = {
      .center = curr_cluster->center & preserve_curr,
      .mat =
          {
              .atten = curr_cluster->mat.atten & preserve_curr,
              .type = _mm256_and_si256(curr_cluster->mat.type, (__m256i)preserve_curr),
          },
      .r = _mm256_and_ps(curr_cluster->r, preserve_curr),

  };

  curr_cluster->center = new_spheres.center + curr_spheres.center;
  curr_cluster->mat.atten = new_spheres.mat.atten + curr_spheres.mat.atten;
  curr_cluster->mat.type = new_spheres.mat.type + curr_spheres.mat.type;
  curr_cluster->r = new_spheres.r + curr_spheres.r;
};

inline static void find_sphere_hits(HitRecords* hit_rec, const RayCluster* rays, float t_max) {

  SphereCluster closest_spheres = {
      .center =
          {
              .x = global::zeros,
              .y = global::zeros,
              .z = global::zeros,
          },
      .r = global::zeros,
  };

  __m256 max = _mm256_broadcast_ss(&global::float_max);

  // find first sphere as a baseline
  __m256 lowest_t_vals = sphere_hit(rays, &spheres[0], t_max);
  __m256 hit_loc = _mm256_cmp_ps(lowest_t_vals, global::zeros, global::cmpneq);

  update_sphere_cluster(&closest_spheres, spheres[0], hit_loc);

  for (size_t i = 1; i < spheres.size(); i++) {
    __m256 new_t_vals = sphere_hit(rays, &spheres[i], t_max);

    // don't update on instances of no hits (hit locations all zeros)
    hit_loc = _mm256_cmp_ps(new_t_vals, global::zeros, global::cmpneq);
    if (_mm256_testz_ps(hit_loc, hit_loc)) {
      continue;
    }

    // replace all 0's with float maximum to not replace actual values with
    // 0's during the minimum comparisons. Again, 0's represent no hits
    __m256 no_hit_loc = _mm256_xor_ps(hit_loc, global::all_set);
    __m256 max_mask = _mm256_and_ps(no_hit_loc, max);
    new_t_vals = _mm256_or_ps(new_t_vals, max_mask);

    // replace 0's with max for current lowest too
    __m256 curr_no_hit_loc = _mm256_cmp_ps(lowest_t_vals, global::zeros, global::cmpeq);
    max_mask = _mm256_and_ps(curr_no_hit_loc, max);
    __m256 lowest_t_masked = _mm256_or_ps(lowest_t_vals, max_mask);

    // update sphere references based on where new
    // t values are closer than the current lowest
    __m256 update_locs = _mm256_cmp_ps(new_t_vals, lowest_t_masked, global::cmplt);
    update_sphere_cluster(&closest_spheres, spheres[i], update_locs);

    // update current lowest t values based on new t's, however, mask out
    // where we put float max values so that the t values still represent
    // no hits as 0.0
    lowest_t_vals = _mm256_min_ps(lowest_t_masked, new_t_vals);
    __m256 actual_vals_loc = _mm256_cmp_ps(lowest_t_vals, max, global::cmpneq);
    lowest_t_vals = _mm256_and_ps(lowest_t_vals, actual_vals_loc);
  }

  create_hit_record(hit_rec, rays, &closest_spheres, lowest_t_vals);
}

#endif