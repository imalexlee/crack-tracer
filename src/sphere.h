#pragma once
#include "constants.h"
#include "types.h"
#include <immintrin.h>

// Returns hit t values or 0 depending on if this ray hit this sphere or not
[[nodiscard]] static inline __m256 sphere_hit(const RayCluster* rays, float t_max, Sphere sphere) {

  // will also copy radius in but thats ok
  __m128 sphere_center = _mm_load_ps(&sphere.center.x);
  __m128 ray_orig = _mm_load_ps(CAM_ORIGIN);
  __m128 oc = _mm_sub_ps(sphere_center, ray_orig);

  __m256 oc_x = _mm256_broadcastss_ps(oc);

  __m128 oc_temp_y = _mm_shuffle_ps(oc, oc, SHUF_ALL_SECOND);
  __m256 oc_y = _mm256_broadcastss_ps(oc_temp_y);

  __m128 oc_temp_z = _mm_shuffle_ps(oc, oc, SHUF_ALL_THIRD);
  __m256 oc_z = _mm256_broadcastss_ps(oc_temp_z);

  __m256 a = _mm256_mul_ps(rays->dir.x, rays->dir.x);
  a = _mm256_fmadd_ps(rays->dir.y, rays->dir.y, a);
  a = _mm256_fmadd_ps(rays->dir.z, rays->dir.z, a);

  __m256 b = _mm256_mul_ps(rays->dir.x, oc_x);
  b = _mm256_fmadd_ps(rays->dir.y, oc_y, b);
  b = _mm256_fmadd_ps(rays->dir.z, oc_z, b);

  __m256 c = _mm256_mul_ps(oc_x, oc_x);
  c = _mm256_fmadd_ps(oc_y, oc_y, c);
  c = _mm256_fmadd_ps(oc_z, oc_z, c);

  float rad_2 = sphere.r * sphere.r;
  __m256 rad_2_vec = _mm256_broadcast_ss(&rad_2);

  c = _mm256_sub_ps(c, rad_2_vec);

  __m256 discrim = _mm256_mul_ps(a, c);
  discrim = _mm256_fmsub_ps(b, b, discrim);

  __m256 zeros = _mm256_setzero_ps();
  __m256 res = _mm256_cmp_ps(discrim, zeros, CMPLTPS);
  int no_hit = _mm256_testz_ps(res, res);

  if (no_hit) {
    return zeros;
  }

  __m256 all_set = _mm256_cmp_ps(res, res, CMPEQPS);
  __m256 hits = _mm256_xor_ps(res, all_set);
  // mask out the discriminants and b where < 0. ie, no hits
  discrim = _mm256_and_ps(discrim, hits);
  b = _mm256_and_ps(b, hits);

  __m256 recip_sqrt_d = _mm256_rsqrt_ps(discrim);
  __m256 sqrt_d = _mm256_mul_ps(recip_sqrt_d, discrim);
  __m256 recip_a = _mm256_rcp_ps(a);

  __m256 root = _mm256_sub_ps(b, sqrt_d);
  root = _mm256_mul_ps(root, recip_a);

  // allow through roots within the max t value
  __m256 t_max_vec = _mm256_broadcast_ss(&t_max);
  res = _mm256_cmp_ps(root, t_max_vec, CMPLTPS);
  root = _mm256_and_ps(root, res);

  return root;
}

[[nodiscard]] static inline HitRecords create_hit_record(const RayCluster* rays, __m256 t_vals,
                                                         SphereCluster* sphere_cluster) {
  HitRecords hit_rec;
  hit_rec.t = t_vals;
  hit_rec.mat_idx = sphere_cluster->mat_idx;

  __m256 dir_xt = _mm256_mul_ps(rays->dir.x, t_vals);
  __m256 dir_yt = _mm256_mul_ps(rays->dir.y, t_vals);
  __m256 dir_zt = _mm256_mul_ps(rays->dir.z, t_vals);

  __m128 ray_orig = _mm_load_ps(CAM_ORIGIN);
  __m256 ray_orig_x = _mm256_broadcastss_ps(ray_orig);

  __m128 ray_orig_temp_y = _mm_shuffle_ps(ray_orig, ray_orig, SHUF_ALL_SECOND);
  __m256 ray_orig_y = _mm256_broadcastss_ps(ray_orig_temp_y);

  __m128 ray_orig_temp_z = _mm_shuffle_ps(ray_orig, ray_orig, SHUF_ALL_THIRD);
  __m256 ray_orig_z = _mm256_broadcastss_ps(ray_orig_temp_z);

  hit_rec.orig.x = _mm256_add_ps(ray_orig_x, dir_xt);
  hit_rec.orig.y = _mm256_add_ps(ray_orig_y, dir_yt);
  hit_rec.orig.z = _mm256_add_ps(ray_orig_z, dir_zt);

  hit_rec.norm.x = _mm256_sub_ps(hit_rec.orig.x, sphere_cluster->center.x);
  hit_rec.norm.y = _mm256_sub_ps(hit_rec.orig.y, sphere_cluster->center.x);
  hit_rec.norm.z = _mm256_sub_ps(hit_rec.orig.z, sphere_cluster->center.x);

  __m256 recip_radius = _mm256_rcp_ps(sphere_cluster->r);

  hit_rec.norm.x = _mm256_mul_ps(hit_rec.norm.x, recip_radius);
  hit_rec.norm.y = _mm256_mul_ps(hit_rec.norm.y, recip_radius);
  hit_rec.norm.z = _mm256_mul_ps(hit_rec.norm.z, recip_radius);

  return hit_rec;
}

/*
  UPDATING SPHERES
  1. create mask where less condition is met
  2. mask new sphere vals to ignore where t's were higher than previous
  3. negate the mask to find where the less condition wasn't met
  4. AND the existing spheres with it to only preserve where it isn't less
  5. add the two masked registers together to form updated sphere cluster
 */
// updates a sphere cluster with a sphere given a mask of where to insert the new sphere's values
static inline void update_sphere_cluster(SphereCluster* curr_cluster, Sphere curr_sphere,
                                         int sphere_idx, __m256 update_mask) {
  __m256 new_sphere_x = _mm256_broadcast_ss(&curr_sphere.center.x);
  __m256 new_sphere_y = _mm256_broadcast_ss(&curr_sphere.center.y);
  __m256 new_sphere_z = _mm256_broadcast_ss(&curr_sphere.center.z);
  __m256 new_sphere_r = _mm256_broadcast_ss(&curr_sphere.r);
  __m256i new_mat_idx = _mm256_set1_epi32(sphere_idx);

  // preserve new sphere values where a new minimum was found
  new_sphere_x = _mm256_and_ps(new_sphere_x, update_mask);
  new_sphere_y = _mm256_and_ps(new_sphere_y, update_mask);
  new_sphere_z = _mm256_and_ps(new_sphere_z, update_mask);
  new_sphere_r = _mm256_and_ps(new_sphere_r, update_mask);
  new_mat_idx = _mm256_and_si256(new_mat_idx, (__m256i)update_mask);

  // set all bits
  __m256 all_set = _mm256_cmp_ps(new_sphere_x, new_sphere_x, CMPEQPS);
  // negation of update locations so we can preserve current values
  // while clearing bits where we will update
  __m256 preserve_curr = _mm256_xor_ps(update_mask, all_set);

  __m256 curr_sphere_x = _mm256_and_ps(curr_cluster->center.x, preserve_curr);
  __m256 curr_sphere_y = _mm256_and_ps(curr_cluster->center.y, preserve_curr);
  __m256 curr_sphere_z = _mm256_and_ps(curr_cluster->center.z, preserve_curr);
  __m256 curr_sphere_r = _mm256_and_ps(curr_cluster->r, preserve_curr);
  __m256i curr_mat_idx = _mm256_and_si256(curr_cluster->mat_idx, (__m256i)preserve_curr);

  curr_cluster->center.x = _mm256_add_ps(new_sphere_x, curr_sphere_x);
  curr_cluster->center.y = _mm256_add_ps(new_sphere_y, curr_sphere_y);
  curr_cluster->center.z = _mm256_add_ps(new_sphere_z, curr_sphere_z);
  curr_cluster->r = _mm256_add_ps(new_sphere_r, curr_sphere_r);
  curr_cluster->mat_idx = _mm256_and_si256(new_mat_idx, curr_mat_idx);
};

[[nodiscard]] static inline HitRecords find_sphere_hits(const Sphere* spheres,
                                                        const RayCluster* rays, float t_max) {
  SphereCluster closest_spheres{
      .center = {.x = _mm256_setzero_ps(), .y = _mm256_setzero_ps(), .z = _mm256_setzero_ps()},
      .r = _mm256_setzero_ps(),
      .mat_idx = _mm256_setzero_si256(),
  };

  __m256 zeros = _mm256_setzero_ps();
  __m256 infs = _mm256_broadcast_ss(&FLOAT_INF);

  // find first sphere as a baseline
  __m256 lowest_t_vals = sphere_hit(rays, t_max, spheres[0]);
  __m256 zero_loc = _mm256_cmp_ps(lowest_t_vals, zeros, CMPEQPS);
  __m256 inf_t_mask = _mm256_and_ps(zero_loc, infs);
  __m256 lower_locs = _mm256_cmp_ps(lowest_t_vals, inf_t_mask, CMPLTPS);
  update_sphere_cluster(&closest_spheres, spheres[0], 0, lower_locs);

  for (int i = 1; i < SPHERE_NUM; i++) {
    __m256 t_vals = sphere_hit(rays, t_max, spheres[i]);
    __m256 zero_loc = _mm256_cmp_ps(t_vals, zeros, CMPEQPS);

    // don't update on instances of no hits
    if (_mm256_testz_ps(zero_loc, zero_loc)) {
      continue;
    }

    __m256 inf_t_mask = _mm256_and_ps(zero_loc, infs);
    __m256 lower_locs = _mm256_cmp_ps(lowest_t_vals, inf_t_mask, CMPLTPS);
    update_sphere_cluster(&closest_spheres, spheres[i], i, lower_locs);

    // use inf where t == 0 in new t values to preserve current
    // values in lowest_t_vals as to not write over actual hit points
    lowest_t_vals = _mm256_min_ps(lowest_t_vals, inf_t_mask);
  }

  return create_hit_record(rays, lowest_t_vals, &closest_spheres);
}
