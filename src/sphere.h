#pragma once
#include "constants.h"
#include "globals.h"
#include "types.h"
#include <array>
#include <immintrin.h>

constexpr Color silver = {
    .r = 0.66f,
    .g = 0.66f,
    .b = 0.66f,
};

constexpr Color red = {
    .r = 0.90f,
    .g = 0.20f,
    .b = 0.20f,
};

constexpr Color gold = {
    .r = 0.85f,
    .g = 0.70f,
    .b = 0.24f,
};

constexpr Material silver_metallic = {.atten = silver, .type = MatType::metallic};
constexpr Material red_metallic = {.atten = red, .type = MatType::metallic};
constexpr Material gold_metallic = {.atten = gold, .type = MatType::lambertian};

constexpr auto spheres = std::to_array<Sphere>({
    {.center = {.x = 0.f, .y = 0.f, .z = -1.2f}, .mat = silver_metallic, .r = 0.5f},
    {.center = {.x = -1.f, .y = 0.f, .z = -1.f}, .mat = red_metallic, .r = 0.4f},
    {.center = {.x = 1.f, .y = 0.f, .z = -1.f}, .mat = gold_metallic, .r = 0.4f},
    //{.center = {.x = 0.f, .y = -100.5f, .z = -1.f}, .mat = silver_metallic, .r = 100.f},
});

// Returns hit t values or 0 depending on if this ray hit this sphere or not
[[nodiscard]] inline static __m256 sphere_hit(const RayCluster* rays, const Sphere* sphere,
                                              const Vec4* cam_origin, float t_max) {

  // will also copy radius in but thats ok
  __m128 sphere_center = _mm_load_ps(&sphere->center.x);
  __m128 ray_orig = _mm_load_ps((float*)cam_origin);
  __m128 oc = _mm_sub_ps(sphere_center, ray_orig);

  __m256 oc_x = _mm256_broadcastss_ps(oc);

  __m128 oc_temp_y = _mm_shuffle_ps(oc, oc, SHUF_ALL_SECOND);
  __m256 oc_y = _mm256_broadcastss_ps(oc_temp_y);

  __m128 oc_temp_z = _mm_shuffle_ps(oc, oc, SHUF_ALL_THIRD);
  __m256 oc_z = _mm256_broadcastss_ps(oc_temp_z);

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
  __m256 hit_loc = _mm256_cmp_ps(discrim, zeros, CMPNLT);
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
  __m256 below_max = _mm256_cmp_ps(root, t_max_vec, CMPLT);
  __m256 above_min = _mm256_cmp_ps(root, zeros, CMPNLT);
  hit_loc = _mm256_and_ps(above_min, below_max);
  root = _mm256_and_ps(root, hit_loc);

  return root;
}

inline static void create_hit_record(HitRecords* hit_rec, const RayCluster* rays,
                                     SphereCluster* sphere_cluster, const Vec4* cam_origin,
                                     __m256 t_vals) {
  hit_rec->t = t_vals;
  hit_rec->mat = sphere_cluster->mat;

  __m256 dir_xt = _mm256_mul_ps(rays->dir.x, t_vals);
  __m256 dir_yt = _mm256_mul_ps(rays->dir.y, t_vals);
  __m256 dir_zt = _mm256_mul_ps(rays->dir.z, t_vals);

  __m128 ray_orig = _mm_load_ps((float*)cam_origin);
  __m256 ray_orig_x = _mm256_broadcastss_ps(ray_orig);

  __m128 ray_orig_temp_y = _mm_shuffle_ps(ray_orig, ray_orig, SHUF_ALL_SECOND);
  __m256 ray_orig_y = _mm256_broadcastss_ps(ray_orig_temp_y);

  __m128 ray_orig_temp_z = _mm_shuffle_ps(ray_orig, ray_orig, SHUF_ALL_THIRD);
  __m256 ray_orig_z = _mm256_broadcastss_ps(ray_orig_temp_z);

  hit_rec->orig.x = _mm256_add_ps(ray_orig_x, dir_xt);
  hit_rec->orig.y = _mm256_add_ps(ray_orig_y, dir_yt);
  hit_rec->orig.z = _mm256_add_ps(ray_orig_z, dir_zt);

  __m256 norm_x = _mm256_sub_ps(hit_rec->orig.x, sphere_cluster->center.x);
  __m256 norm_y = _mm256_sub_ps(hit_rec->orig.y, sphere_cluster->center.y);
  __m256 norm_z = _mm256_sub_ps(hit_rec->orig.z, sphere_cluster->center.z);

  __m256 recip_radius = _mm256_rcp_ps(sphere_cluster->r);

  // normalize
  hit_rec->norm.x = _mm256_mul_ps(norm_x, recip_radius);
  hit_rec->norm.y = _mm256_mul_ps(norm_y, recip_radius);
  hit_rec->norm.z = _mm256_mul_ps(norm_z, recip_radius);
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

inline static void find_sphere_hits(HitRecords* hit_rec, const RayCluster* rays,
                                    const Vec4* cam_origin, float t_max) {

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

  __m256 max = _mm256_broadcast_ss(&FLOAT_MAX);

  // find first sphere as a baseline
  __m256 lowest_t_vals = sphere_hit(rays, &spheres[0], cam_origin, t_max);
  __m256 hit_loc = _mm256_cmp_ps(lowest_t_vals, zeros, CMPNEQ);

  update_sphere_cluster(&closest_spheres, spheres[0], hit_loc);

  for (size_t i = 1; i < spheres.size(); i++) {
    __m256 new_t_vals = sphere_hit(rays, &spheres[i], cam_origin, t_max);

    // don't update on instances of no hits (hit locations all zeros)
    hit_loc = _mm256_cmp_ps(new_t_vals, zeros, CMPNEQ);
    if (_mm256_testz_ps(hit_loc, hit_loc)) {
      continue;
    }

    // replace all 0's with float maximum to not replace actual values with
    // 0's during the minimum comparisons. Again, 0's represent no hits
    __m256 no_hit_loc = _mm256_xor_ps(hit_loc, global::all_set);
    __m256 max_mask = _mm256_and_ps(no_hit_loc, max);
    new_t_vals = _mm256_or_ps(new_t_vals, max_mask);

    // replace 0's with max for current lowest too
    __m256 curr_no_hit_loc = _mm256_cmp_ps(lowest_t_vals, zeros, CMPEQ);
    max_mask = _mm256_and_ps(curr_no_hit_loc, max);
    __m256 lowest_t_masked = _mm256_or_ps(lowest_t_vals, max_mask);

    // update sphere references based on where new
    // t values are closer than the current lowest
    __m256 update_locs = _mm256_cmp_ps(new_t_vals, lowest_t_masked, CMPLT);
    update_sphere_cluster(&closest_spheres, spheres[i], update_locs);

    // update current lowest t values based on new t's, however, mask out
    // where we put float max values so that the t values still represent
    // no hits as 0.0
    lowest_t_vals = _mm256_min_ps(lowest_t_masked, new_t_vals);
    __m256 actual_vals_loc = _mm256_cmp_ps(lowest_t_vals, max, CMPNEQ);
    lowest_t_vals = _mm256_and_ps(lowest_t_vals, actual_vals_loc);
  }

  create_hit_record(hit_rec, rays, &closest_spheres, cam_origin, lowest_t_vals);
}
