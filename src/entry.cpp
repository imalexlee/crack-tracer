#include <array>
#include <cstdint>
#include <cstdio>
#include <immintrin.h>

constexpr uint8_t SPHERE_NUM = 1;
constexpr uint8_t CMPNLTPS = 5;
constexpr uint8_t CMPLTPS = 1;
constexpr uint8_t SHUF_ALL_FIRST = 0;
constexpr uint8_t SHUF_ALL_SECOND = 85;
constexpr uint8_t SHUF_ALL_THIRD = 170;
alignas(32) constexpr float CAM_ORIGIN[4] = {0.f, 0.f, 0.f, 0.f};

struct Vec3 {
  float x;
  float y;
  float z;
};

struct alignas(32) Sphere {
  float center[4];
  float radius;
};

void print_vec(__m256 vec) {
  printf("%f %f %f %f %f %f %f %f\n", vec[0], vec[1], vec[2], vec[3], vec[4], vec[5], vec[6],
         vec[7]);
}

constexpr auto spheres{[]() constexpr {
  std::array<Sphere, SPHERE_NUM> result;
  constexpr Sphere new_sphere{
      .center = {0.f, 0.f, -1.f, 0.f},
      .radius = 0.5f,
  };
  result[0] = new_sphere;
  return result;
}()};

struct RayCluster {
  __m256 dir_x;
  __m256 dir_y;
  __m256 dir_z;
};

struct HitRecord {
  __m256 t;
  __m256 orig_x;
  __m256 orig_y;
  __m256 orig_z;
  __m256 norm_x;
  __m256 norm_y;
  __m256 norm_z;
};

// Returns hit t values or 0 depending on if this ray hit this sphere
__m256 sphere_hit(const RayCluster& rays, float t_max, Sphere sphere) {

  __m128 sphere_center = _mm_load_ps(sphere.center);
  __m128 ray_orig = _mm_load_ps(CAM_ORIGIN);
  __m128 oc = _mm_sub_ps(sphere_center, ray_orig);

  __m256 oc_x = _mm256_broadcastss_ps(oc);

  __m128 oc_temp_y = _mm_shuffle_ps(oc, oc, SHUF_ALL_SECOND);
  __m256 oc_y = _mm256_broadcastss_ps(oc_temp_y);

  __m128 oc_temp_z = _mm_shuffle_ps(oc, oc, SHUF_ALL_THIRD);
  __m256 oc_z = _mm256_broadcastss_ps(oc_temp_z);

  __m256 a = _mm256_mul_ps(rays.dir_x, rays.dir_x);
  a = _mm256_fmadd_ps(rays.dir_y, rays.dir_y, a);
  a = _mm256_fmadd_ps(rays.dir_z, rays.dir_z, a);

  __m256 b = _mm256_mul_ps(rays.dir_x, oc_x);
  b = _mm256_fmadd_ps(rays.dir_y, oc_y, b);
  b = _mm256_fmadd_ps(rays.dir_z, oc_z, b);

  __m256 c = _mm256_mul_ps(oc_x, oc_x);
  c = _mm256_fmadd_ps(oc_y, oc_y, c);
  c = _mm256_fmadd_ps(oc_z, oc_z, c);

  float rad_2 = sphere.radius * sphere.radius;
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

HitRecord create_hit_record(const RayCluster& rays, __m256 t_vals, Sphere sphere) {
  HitRecord hit_rec;
  hit_rec.t = t_vals;

  __m256 dir_xt = _mm256_mul_ps(rays.dir_x, t_vals);
  __m256 dir_yt = _mm256_mul_ps(rays.dir_y, t_vals);
  __m256 dir_zt = _mm256_mul_ps(rays.dir_z, t_vals);

  __m128 ray_orig = _mm_load_ps(CAM_ORIGIN);
  __m256 ray_orig_x = _mm256_broadcastss_ps(ray_orig);

  __m128 ray_orig_temp_y = _mm_shuffle_ps(ray_orig, ray_orig, SHUF_ALL_SECOND);
  __m256 ray_orig_y = _mm256_broadcastss_ps(ray_orig_temp_y);

  __m128 ray_orig_temp_z = _mm_shuffle_ps(ray_orig, ray_orig, SHUF_ALL_THIRD);
  __m256 ray_orig_z = _mm256_broadcastss_ps(ray_orig_temp_z);

  hit_rec.orig_x = _mm256_add_ps(ray_orig_x, dir_xt);
  hit_rec.orig_y = _mm256_add_ps(ray_orig_y, dir_yt);
  hit_rec.orig_z = _mm256_add_ps(ray_orig_z, dir_zt);

  __m128 sphere_center = _mm_load_ps(sphere.center);
  __m256 sphere_center_x = _mm256_broadcastss_ps(sphere_center);

  __m128 sphere_center_temp_y = _mm_shuffle_ps(sphere_center, sphere_center, SHUF_ALL_SECOND);
  __m256 sphere_center_y = _mm256_broadcastss_ps(sphere_center_temp_y);

  __m128 sphere_center_temp_z = _mm_shuffle_ps(sphere_center, sphere_center, SHUF_ALL_THIRD);
  __m256 sphere_center_z = _mm256_broadcastss_ps(sphere_center_temp_z);

  hit_rec.norm_x = _mm256_sub_ps(hit_rec.orig_x, sphere_center_x);
  hit_rec.norm_y = _mm256_sub_ps(hit_rec.orig_y, sphere_center_y);
  hit_rec.norm_z = _mm256_sub_ps(hit_rec.orig_z, sphere_center_z);

  __m256 recip_radius = _mm256_broadcast_ss(&sphere.radius);
  recip_radius = _mm256_rcp_ps(recip_radius);

  hit_rec.norm_x = _mm256_mul_ps(hit_rec.norm_x, recip_radius);
  hit_rec.norm_y = _mm256_mul_ps(hit_rec.norm_y, recip_radius);
  hit_rec.norm_z = _mm256_mul_ps(hit_rec.norm_z, recip_radius);

  return hit_rec;
}

int main() {

  RayCluster rays = {
      .dir_x = _mm256_setzero_ps(),
      .dir_y = _mm256_setzero_ps(),
      .dir_z = {-1.f, -1.f, -1.f, -1.f, -1.f, -1.f, -1.f, -1.f},
  };
  __m256 t_vals = sphere_hit(rays, 100, spheres[0]);
  create_hit_record(rays, t_vals, spheres[0]);

  return 0;
}
