
#include "sphere.h"
#include "constants.h"
#include "globals.h"
#include "types.h"

// Returns hit t values or 0 depending on if this ray hit this sphere or not
__m256 sphere_hit(const RayCluster* rays, float t_max, Sphere sphere) {

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

void create_hit_record(HitRecords* hit_rec, const RayCluster* rays, __m256 t_vals,
                       SphereCluster* sphere_cluster) {
  hit_rec->t = t_vals;
  hit_rec->mat = sphere_cluster->mat;

  __m256 dir_xt = _mm256_mul_ps(rays->dir.x, t_vals);
  __m256 dir_yt = _mm256_mul_ps(rays->dir.y, t_vals);
  __m256 dir_zt = _mm256_mul_ps(rays->dir.z, t_vals);

  __m128 ray_orig = _mm_load_ps(CAM_ORIGIN);
  __m256 ray_orig_x = _mm256_broadcastss_ps(ray_orig);

  __m128 ray_orig_temp_y = _mm_shuffle_ps(ray_orig, ray_orig, SHUF_ALL_SECOND);
  __m256 ray_orig_y = _mm256_broadcastss_ps(ray_orig_temp_y);

  __m128 ray_orig_temp_z = _mm_shuffle_ps(ray_orig, ray_orig, SHUF_ALL_THIRD);
  __m256 ray_orig_z = _mm256_broadcastss_ps(ray_orig_temp_z);

  hit_rec->orig.x = _mm256_add_ps(ray_orig_x, dir_xt);
  hit_rec->orig.y = _mm256_add_ps(ray_orig_y, dir_yt);
  hit_rec->orig.z = _mm256_add_ps(ray_orig_z, dir_zt);

  hit_rec->norm.x = _mm256_sub_ps(hit_rec->orig.x, sphere_cluster->center.x);
  hit_rec->norm.y = _mm256_sub_ps(hit_rec->orig.y, sphere_cluster->center.y);
  hit_rec->norm.z = _mm256_sub_ps(hit_rec->orig.z, sphere_cluster->center.z);

  __m256 recip_radius = _mm256_rcp_ps(sphere_cluster->r);

  hit_rec->norm.x = _mm256_mul_ps(hit_rec->norm.x, recip_radius);
  hit_rec->norm.y = _mm256_mul_ps(hit_rec->norm.y, recip_radius);
  hit_rec->norm.z = _mm256_mul_ps(hit_rec->norm.z, recip_radius);
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
void update_sphere_cluster(SphereCluster* curr_cluster, Sphere curr_sphere, __m256 update_mask) {
  __m256 new_sphere_x = _mm256_broadcast_ss(&curr_sphere.center.x);
  __m256 new_sphere_y = _mm256_broadcast_ss(&curr_sphere.center.y);
  __m256 new_sphere_z = _mm256_broadcast_ss(&curr_sphere.center.z);
  __m256 new_sphere_r = _mm256_broadcast_ss(&curr_sphere.r);
  __m256 new_sphere_atten_r = _mm256_broadcast_ss(&curr_sphere.mat.atten.r);
  __m256 new_sphere_atten_g = _mm256_broadcast_ss(&curr_sphere.mat.atten.g);
  __m256 new_sphere_atten_b = _mm256_broadcast_ss(&curr_sphere.mat.atten.b);

  // preserve new sphere values where a new minimum was found
  new_sphere_x = _mm256_and_ps(new_sphere_x, update_mask);
  new_sphere_y = _mm256_and_ps(new_sphere_y, update_mask);
  new_sphere_z = _mm256_and_ps(new_sphere_z, update_mask);
  new_sphere_r = _mm256_and_ps(new_sphere_r, update_mask);
  new_sphere_atten_r = _mm256_and_ps(new_sphere_atten_r, update_mask);
  new_sphere_atten_g = _mm256_and_ps(new_sphere_atten_g, update_mask);
  new_sphere_atten_b = _mm256_and_ps(new_sphere_atten_b, update_mask);

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

  curr_cluster->center.x = _mm256_add_ps(new_sphere_x, curr_sphere_x);
  curr_cluster->center.y = _mm256_add_ps(new_sphere_y, curr_sphere_y);
  curr_cluster->center.z = _mm256_add_ps(new_sphere_z, curr_sphere_z);
  curr_cluster->r = _mm256_add_ps(new_sphere_r, curr_sphere_r);
  curr_cluster->mat.atten.r = _mm256_add_ps(new_sphere_atten_r, curr_sphere_atten_r);
  curr_cluster->mat.atten.g = _mm256_add_ps(new_sphere_atten_g, curr_sphere_atten_g);
  curr_cluster->mat.atten.b = _mm256_add_ps(new_sphere_atten_b, curr_sphere_atten_b);
};

void find_sphere_hits(SphereCluster* sphere_cluster, HitRecords* hit_rec, const Sphere* spheres,
                      const RayCluster* rays, float t_max) {

  __m256 zeros = _mm256_setzero_ps();

  __m256 max = _mm256_broadcast_ss(&FLOAT_MAX);

  // find first sphere as a baseline
  __m256 lowest_t_vals = sphere_hit(rays, t_max, spheres[0]);
  __m256 hit_loc = _mm256_cmp_ps(lowest_t_vals, zeros, CMPNEQ);
  update_sphere_cluster(sphere_cluster, spheres[0], hit_loc);

  for (int i = 1; i < SPHERE_NUM; i++) {
    __m256 new_t_vals = sphere_hit(rays, t_max, spheres[i]);

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
    update_sphere_cluster(sphere_cluster, spheres[i], update_locs);

    // update current lowest t values based on new t's, however, mask out
    // where we put float max values so that the t values still represent
    // no hits as 0.0
    lowest_t_vals = _mm256_min_ps(lowest_t_masked, new_t_vals);
    __m256 actual_vals_loc = _mm256_cmp_ps(lowest_t_vals, max, CMPNEQ);
    lowest_t_vals = _mm256_and_ps(lowest_t_vals, actual_vals_loc);
  }

  create_hit_record(hit_rec, rays, lowest_t_vals, sphere_cluster);
}
