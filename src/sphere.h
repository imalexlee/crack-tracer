#pragma once
#include "constants.h"
#include "types.h"
#include <immintrin.h>

// Returns hit t values or 0 depending on if this ray hit this sphere or not
static inline __m256 sphere_hit(const RayCluster* rays, float t_max, Sphere sphere) {

  // will also copy radius in but thats ok
  __m128 sphere_center = _mm_load_ps(&sphere.x);
  __m128 ray_orig = _mm_load_ps(CAM_ORIGIN);
  __m128 oc = _mm_sub_ps(sphere_center, ray_orig);

  __m256 oc_x = _mm256_broadcastss_ps(oc);

  __m128 oc_temp_y = _mm_shuffle_ps(oc, oc, SHUF_ALL_SECOND);
  __m256 oc_y = _mm256_broadcastss_ps(oc_temp_y);

  __m128 oc_temp_z = _mm_shuffle_ps(oc, oc, SHUF_ALL_THIRD);
  __m256 oc_z = _mm256_broadcastss_ps(oc_temp_z);

  __m256 a = _mm256_mul_ps(rays->dir_x, rays->dir_x);
  a = _mm256_fmadd_ps(rays->dir_y, rays->dir_y, a);
  a = _mm256_fmadd_ps(rays->dir_z, rays->dir_z, a);

  __m256 b = _mm256_mul_ps(rays->dir_x, oc_x);
  b = _mm256_fmadd_ps(rays->dir_y, oc_y, b);
  b = _mm256_fmadd_ps(rays->dir_z, oc_z, b);

  __m256 c = _mm256_mul_ps(oc_x, oc_x);
  c = _mm256_fmadd_ps(oc_y, oc_y, c);
  c = _mm256_fmadd_ps(oc_z, oc_z, c);

  float rad_2 = sphere.r * sphere.r;
  __m256 rad_2_vec = _mm256_broadcast_ss(&rad_2);

  c = _mm256_sub_ps(c, rad_2_vec);

  __m256 discrim = _mm256_mul_ps(a, c);
  discrim = _mm256_fmsub_ps(b, b, discrim);

  __m256 zeros = _mm256_setzero_ps();
  __m256 res = _mm256_cmp_ps(discrim, zeros, CMPNLTPS);
  int no_hit = _mm256_testz_ps(res, res);

  if (no_hit) {
    return zeros;
  }

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

static inline HitRecords create_hit_record(const RayCluster* rays, __m256 t_vals,
                                           SphereCluster* sphere_cluster) {
  HitRecords hit_rec;
  hit_rec.t = t_vals;

  __m256 dir_xt = _mm256_mul_ps(rays->dir_x, t_vals);
  __m256 dir_yt = _mm256_mul_ps(rays->dir_y, t_vals);
  __m256 dir_zt = _mm256_mul_ps(rays->dir_z, t_vals);

  __m128 ray_orig = _mm_load_ps(CAM_ORIGIN);
  __m256 ray_orig_x = _mm256_broadcastss_ps(ray_orig);

  __m128 ray_orig_temp_y = _mm_shuffle_ps(ray_orig, ray_orig, SHUF_ALL_SECOND);
  __m256 ray_orig_y = _mm256_broadcastss_ps(ray_orig_temp_y);

  __m128 ray_orig_temp_z = _mm_shuffle_ps(ray_orig, ray_orig, SHUF_ALL_THIRD);
  __m256 ray_orig_z = _mm256_broadcastss_ps(ray_orig_temp_z);

  hit_rec.orig_x = _mm256_add_ps(ray_orig_x, dir_xt);
  hit_rec.orig_y = _mm256_add_ps(ray_orig_y, dir_yt);
  hit_rec.orig_z = _mm256_add_ps(ray_orig_z, dir_zt);

  hit_rec.norm_x = _mm256_sub_ps(hit_rec.orig_x, sphere_cluster->x);
  hit_rec.norm_y = _mm256_sub_ps(hit_rec.orig_y, sphere_cluster->y);
  hit_rec.norm_z = _mm256_sub_ps(hit_rec.orig_z, sphere_cluster->z);

  __m256 recip_radius = _mm256_rcp_ps(sphere_cluster->r);

  hit_rec.norm_x = _mm256_mul_ps(hit_rec.norm_x, recip_radius);
  hit_rec.norm_y = _mm256_mul_ps(hit_rec.norm_y, recip_radius);
  hit_rec.norm_z = _mm256_mul_ps(hit_rec.norm_z, recip_radius);

  return hit_rec;
}

/*
  UPDATING SPHERES
  1. where less condition is met, set FFFF
  2. create mask var of new sphere vals where FFFF. AND it
  3. not the less condition variable
  4. AND the existing spheres with it to only preserve where it isn't less
  5. add the masked var of new sphere vals and old sphere vals
 */
// updates a sphere cluster with a sphere given a mask of where to insert the new sphere's values
static inline void update_sphere_cluster(SphereCluster* curr_cluster, Sphere curr_sphere,
                                         __m256 lower_locs) {
  __m256 new_spheres_x = _mm256_broadcast_ss(&curr_sphere.x);
  __m256 new_spheres_y = _mm256_broadcast_ss(&curr_sphere.y);
  __m256 new_spheres_z = _mm256_broadcast_ss(&curr_sphere.z);
  __m256 new_spheres_r = _mm256_broadcast_ss(&curr_sphere.r);

  // preserve new sphere values where a new minimum was found
  new_spheres_x = _mm256_and_ps(new_spheres_x, lower_locs);
  new_spheres_y = _mm256_and_ps(new_spheres_y, lower_locs);
  new_spheres_z = _mm256_and_ps(new_spheres_z, lower_locs);
  new_spheres_r = _mm256_and_ps(new_spheres_r, lower_locs);

  // use any random registers to generate vector of all 0xFFFF's
  __m256 all_set = _mm256_cmp_ps(new_spheres_x, new_spheres_x, CMPEQPS);
  // negation of lower locations used to mask current closest spheres
  __m256 higher_locs = _mm256_xor_ps(lower_locs, all_set);

  __m256 curr_sphere_x = _mm256_and_ps(curr_cluster->x, higher_locs);
  __m256 curr_sphere_y = _mm256_and_ps(curr_cluster->y, higher_locs);
  __m256 curr_sphere_z = _mm256_and_ps(curr_cluster->z, higher_locs);
  __m256 curr_sphere_r = _mm256_and_ps(curr_cluster->r, higher_locs);

  new_spheres_x = _mm256_add_ps(new_spheres_x, curr_sphere_x);
  new_spheres_y = _mm256_add_ps(new_spheres_y, curr_sphere_y);
  new_spheres_z = _mm256_add_ps(new_spheres_z, curr_sphere_z);
  new_spheres_r = _mm256_add_ps(new_spheres_r, curr_sphere_r);

  curr_cluster->x = new_spheres_x;
  curr_cluster->y = new_spheres_y;
  curr_cluster->z = new_spheres_z;
  curr_cluster->r = new_spheres_r;
};

static inline HitRecords find_sphere_hits(const Sphere* spheres, const RayCluster* rays,
                                          float t_max) {
  SphereCluster closest_spheres;
  __m256 zeros = _mm256_setzero_ps();
  __m256 infs = _mm256_broadcast_ss(&FLOAT_INF);

  // find first sphere as a baseline
  __m256 lowest_t_vals = sphere_hit(rays, t_max, spheres[0]);
  __m256 zero_loc = _mm256_cmp_ps(lowest_t_vals, zeros, CMPEQPS);
  __m256 inf_t_mask = _mm256_and_ps(zero_loc, infs);
  __m256 lower_locs = _mm256_cmp_ps(lowest_t_vals, inf_t_mask, CMPLTPS);
  update_sphere_cluster(&closest_spheres, spheres[0], lower_locs);

  for (int i = 1; i < SPHERE_NUM; i++) {
    __m256 t_vals = sphere_hit(rays, t_max, spheres[i]);
    __m256 zero_loc = _mm256_cmp_ps(t_vals, zeros, CMPEQPS);

    // don't update on instances of no hits
    if (_mm256_testz_ps(zero_loc, zero_loc)) {
      continue;
    }

    __m256 inf_t_mask = _mm256_and_ps(zero_loc, infs);
    __m256 lower_locs = _mm256_cmp_ps(lowest_t_vals, inf_t_mask, CMPLTPS);
    update_sphere_cluster(&closest_spheres, spheres[i], lower_locs);

    // use inf where t == 0 in new t values to preserve current
    // values in lowest_t_vals as to not write over actual hit points
    lowest_t_vals = _mm256_min_ps(lowest_t_vals, inf_t_mask);
  }

  return create_hit_record(rays, lowest_t_vals, &closest_spheres);
}
