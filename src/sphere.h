#pragma once
#include "materials.h"
#include "math.h"
#include "rand.h"
#include "types.h"
#include <immintrin.h>
#include <vector>

static std::vector<Sphere> spheres;
inline static void init_spheres() {
  spheres.reserve(300);
  spheres = {
      {{.center = {.x = -1.f, .y = 1.f, .z = -2.5f}, .mat = red_lambertian, .r = 1.f},
       {.center = {.x = 0.f, .y = 1.f, .z = 0.f}, .mat = glass, .r = 1.f},
       {.center = {.x = 1.f, .y = 1.f, .z = 2.5f}, .mat = copper_metallic, .r = 1.f},
       {.center = {.x = 0.f, .y = -1000.f, .z = 0.f}, .mat = grey_lambertian, .r = 1000.f}},
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
            .r = lcg_rand.rand_in_range(0, 1),
            .g = lcg_rand.rand_in_range(0, 1),
            .b = lcg_rand.rand_in_range(0, 1),
        };
        Material new_mat = {.atten = albedo, .type = MatType::lambertian};
        spheres.push_back(Sphere{.center = center, .mat = new_mat, .r = 0.2});
      } else if (choose_mat < 0.7) {
        // metal
        Color albedo = {
            .r = lcg_rand.rand_in_range(0.5, 1),
            .g = lcg_rand.rand_in_range(0.5, 1),
            .b = lcg_rand.rand_in_range(0.5, 1),
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

  spheres.shrink_to_fit();
}

// Returns hit t values or 0 depending on if this ray hit this sphere or not
[[nodiscard]] inline static __m256 sphere_hit(const RayCluster* rays, const Sphere* sphere,
                                              float t_max) {

  __m256 sphere_center_x = _mm256_broadcast_ss(&sphere->center.x);
  __m256 oc_x = _mm256_sub_ps(sphere_center_x, rays->orig.x);

  __m256 sphere_center_y = _mm256_broadcast_ss(&sphere->center.y);
  __m256 oc_y = _mm256_sub_ps(sphere_center_y, rays->orig.y);

  __m256 sphere_center_z = _mm256_broadcast_ss(&sphere->center.z);
  __m256 oc_z = _mm256_sub_ps(sphere_center_z, rays->orig.z);

  float rad_2 = sphere->r * sphere->r;
  __m256 rad_2_vec = _mm256_broadcast_ss(&rad_2);

  __m256 a = _mm256_mul_ps(rays->dir.x, rays->dir.x);
  a = _mm256_fmadd_ps(rays->dir.y, rays->dir.y, a);
  a = _mm256_fmadd_ps(rays->dir.z, rays->dir.z, a);

  __m256 b = _mm256_mul_ps(rays->dir.x, oc_x);
  b = _mm256_fmadd_ps(rays->dir.y, oc_y, b);
  b = _mm256_fmadd_ps(rays->dir.z, oc_z, b);

  __m256 c = _mm256_mul_ps(oc_x, oc_x);
  c = _mm256_fmadd_ps(oc_y, oc_y, c);
  c = _mm256_fmadd_ps(oc_z, oc_z, c);
  c = _mm256_sub_ps(c, rad_2_vec);

  __m256 discrim = _mm256_mul_ps(a, c);
  discrim = _mm256_fmsub_ps(b, b, discrim);

  __m256 zeros = _mm256_setzero_ps();
  __m256 hit_loc = _mm256_cmp_ps(discrim, zeros, global::cmpnlt);
  int no_hit = _mm256_testz_ps(hit_loc, hit_loc);

  if (no_hit) {
    return zeros;
  }

  // mask out the discriminants and b where there aren't hits
  discrim = _mm256_and_ps(discrim, hit_loc);
  b = _mm256_and_ps(b, hit_loc);

  __m256 recip_sqrt_d = _mm256_rsqrt_ps(discrim);
  __m256 sqrt_d = _mm256_mul_ps(recip_sqrt_d, discrim);
  __m256 recip_a = _mm256_rcp_ps(a);

  __m256 root = _mm256_sub_ps(b, sqrt_d);
  root = _mm256_mul_ps(root, recip_a);

  // allow through roots within the max t value
  __m256 t_max_vec = _mm256_broadcast_ss(&t_max);
  __m256 below_max = _mm256_cmp_ps(root, t_max_vec, global::cmplt);
  __m256 above_min = _mm256_cmp_ps(root, global::t_min_vec, global::cmpnlt);
  hit_loc = _mm256_and_ps(above_min, below_max);
  root = _mm256_and_ps(root, hit_loc);

  return root;
}

inline static void set_face_normal(const RayCluster* rays, HitRecords* hit_rec,
                                   Vec3_256* outward_norm) {

  hit_rec->norm = *outward_norm;

  __m256 ray_norm_dot = dot(&rays->dir, outward_norm);
  hit_rec->front_face = _mm256_cmp_ps(ray_norm_dot, _mm256_setzero_ps(), global::cmplt);
  __m256 back_face = _mm256_xor_ps(hit_rec->front_face, global::all_set);
  if (_mm256_testz_ps(back_face, back_face)) {
    return;
  }

  __m256 invert = _mm256_sub_ps(_mm256_setzero_ps(), global::white);

  Vec3_256 inward_norm = {
      .x = _mm256_mul_ps(outward_norm->x, invert),
      .y = _mm256_mul_ps(outward_norm->y, invert),
      .z = _mm256_mul_ps(outward_norm->z, invert),
  };

  hit_rec->norm.x = _mm256_blendv_ps(hit_rec->norm.x, inward_norm.x, back_face);
  hit_rec->norm.y = _mm256_blendv_ps(hit_rec->norm.y, inward_norm.y, back_face);
  hit_rec->norm.z = _mm256_blendv_ps(hit_rec->norm.z, inward_norm.z, back_face);
}

inline static void create_hit_record(HitRecords* hit_rec, const RayCluster* rays,
                                     SphereCluster* sphere_cluster, __m256 t_vals) {
  hit_rec->t = t_vals;
  hit_rec->mat = sphere_cluster->mat;

  __m256 dir_xt = _mm256_mul_ps(rays->dir.x, t_vals);
  __m256 dir_yt = _mm256_mul_ps(rays->dir.y, t_vals);
  __m256 dir_zt = _mm256_mul_ps(rays->dir.z, t_vals);

  __m256 ray_orig_x = rays->orig.x;
  __m256 ray_orig_y = rays->orig.y;
  __m256 ray_orig_z = rays->orig.z;

  hit_rec->orig.x = _mm256_add_ps(ray_orig_x, dir_xt);
  hit_rec->orig.y = _mm256_add_ps(ray_orig_y, dir_yt);
  hit_rec->orig.z = _mm256_add_ps(ray_orig_z, dir_zt);

  Vec3_256 norm = {
      .x = _mm256_sub_ps(hit_rec->orig.x, sphere_cluster->center.x),
      .y = _mm256_sub_ps(hit_rec->orig.y, sphere_cluster->center.y),
      .z = _mm256_sub_ps(hit_rec->orig.z, sphere_cluster->center.z),
  };

  __m256 recip_radius = _mm256_rcp_ps(sphere_cluster->r);

  // normalize
  norm.x = _mm256_mul_ps(norm.x, recip_radius);
  norm.y = _mm256_mul_ps(norm.y, recip_radius);
  norm.z = _mm256_mul_ps(norm.z, recip_radius);

  // hit_rec->norm = norm;

  set_face_normal(rays, hit_rec, &norm);
}

// updates a sphere cluster with a sphere given a mask of where to insert the new sphere's values
inline static void update_sphere_cluster(SphereCluster* curr_cluster, Sphere curr_sphere,
                                         __m256 update_mask) {

  if (_mm256_testz_ps(update_mask, update_mask)) {
    return;
  }

  __m256 new_sphere_x = _mm256_broadcast_ss(&curr_sphere.center.x);
  __m256 new_sphere_y = _mm256_broadcast_ss(&curr_sphere.center.y);
  __m256 new_sphere_z = _mm256_broadcast_ss(&curr_sphere.center.z);
  __m256 new_sphere_r = _mm256_broadcast_ss(&curr_sphere.r);
  __m256 new_sphere_atten_r = _mm256_broadcast_ss(&curr_sphere.mat.atten.r);
  __m256 new_sphere_atten_g = _mm256_broadcast_ss(&curr_sphere.mat.atten.g);
  __m256 new_sphere_atten_b = _mm256_broadcast_ss(&curr_sphere.mat.atten.b);
  __m256i new_sphere_mat_type = _mm256_set1_epi32(curr_sphere.mat.type);

  // preserve new sphere values where a new minimum was found
  new_sphere_x = _mm256_and_ps(new_sphere_x, update_mask);
  new_sphere_y = _mm256_and_ps(new_sphere_y, update_mask);
  new_sphere_z = _mm256_and_ps(new_sphere_z, update_mask);
  new_sphere_r = _mm256_and_ps(new_sphere_r, update_mask);
  new_sphere_atten_r = _mm256_and_ps(new_sphere_atten_r, update_mask);
  new_sphere_atten_g = _mm256_and_ps(new_sphere_atten_g, update_mask);
  new_sphere_atten_b = _mm256_and_ps(new_sphere_atten_b, update_mask);
  new_sphere_mat_type = _mm256_and_si256(new_sphere_mat_type, (__m256i)update_mask);

  // negation of update locations so we can preserve current values
  // while clearing bits where we will update
  __m256 preserve_curr = _mm256_xor_ps(update_mask, global::all_set);

  __m256 curr_sphere_x = _mm256_and_ps(curr_cluster->center.x, preserve_curr);
  __m256 curr_sphere_y = _mm256_and_ps(curr_cluster->center.y, preserve_curr);
  __m256 curr_sphere_z = _mm256_and_ps(curr_cluster->center.z, preserve_curr);
  __m256 curr_sphere_r = _mm256_and_ps(curr_cluster->r, preserve_curr);
  __m256 curr_sphere_atten_r = _mm256_and_ps(curr_cluster->mat.atten.r, preserve_curr);
  __m256 curr_sphere_atten_g = _mm256_and_ps(curr_cluster->mat.atten.g, preserve_curr);
  __m256 curr_sphere_atten_b = _mm256_and_ps(curr_cluster->mat.atten.b, preserve_curr);
  __m256i curr_sphere_mat_type = _mm256_and_si256(curr_cluster->mat.type, (__m256i)preserve_curr);

  curr_cluster->center.x = _mm256_add_ps(new_sphere_x, curr_sphere_x);
  curr_cluster->center.y = _mm256_add_ps(new_sphere_y, curr_sphere_y);
  curr_cluster->center.z = _mm256_add_ps(new_sphere_z, curr_sphere_z);
  curr_cluster->r = _mm256_add_ps(new_sphere_r, curr_sphere_r);
  curr_cluster->mat.atten.r = _mm256_add_ps(new_sphere_atten_r, curr_sphere_atten_r);
  curr_cluster->mat.atten.g = _mm256_add_ps(new_sphere_atten_g, curr_sphere_atten_g);
  curr_cluster->mat.atten.b = _mm256_add_ps(new_sphere_atten_b, curr_sphere_atten_b);
  curr_cluster->mat.type = _mm256_add_epi32(new_sphere_mat_type, curr_sphere_mat_type);
};

inline static void find_sphere_hits(HitRecords* hit_rec, const RayCluster* rays, float t_max) {

  __m256 zeros = _mm256_setzero_ps();
  SphereCluster closest_spheres = {
      .center =
          {
              .x = zeros,
              .y = zeros,
              .z = zeros,
          },
      .r = zeros,
  };

  __m256 max = _mm256_broadcast_ss(&global::float_max);

  // find first sphere as a baseline
  __m256 lowest_t_vals = sphere_hit(rays, &spheres[0], t_max);
  __m256 hit_loc = _mm256_cmp_ps(lowest_t_vals, zeros, global::cmpneq);

  update_sphere_cluster(&closest_spheres, spheres[0], hit_loc);

  for (size_t i = 1; i < spheres.size(); i++) {
    __m256 new_t_vals = sphere_hit(rays, &spheres[i], t_max);

    // don't update on instances of no hits (hit locations all zeros)
    hit_loc = _mm256_cmp_ps(new_t_vals, zeros, global::cmpneq);
    if (_mm256_testz_ps(hit_loc, hit_loc)) {
      continue;
    }

    // replace all 0's with float maximum to not replace actual values with
    // 0's during the minimum comparisons. Again, 0's represent no hits
    __m256 no_hit_loc = _mm256_xor_ps(hit_loc, global::all_set);
    __m256 max_mask = _mm256_and_ps(no_hit_loc, max);
    new_t_vals = _mm256_or_ps(new_t_vals, max_mask);

    // replace 0's with max for current lowest too
    __m256 curr_no_hit_loc = _mm256_cmp_ps(lowest_t_vals, zeros, global::cmpeq);
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
