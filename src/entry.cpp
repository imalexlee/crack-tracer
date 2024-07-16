#include "constants.h"
#include "sphere.h"
#include "types.h"
#include <csignal>
#include <cstdio>
#include <immintrin.h>

constexpr Sphere spheres[SPHERE_NUM] = {{
    .x = 0.f,
    .y = 0.f,
    .z = 0.f,
    .r = 0.f,
}};

constexpr RayColors silver_attenuation = {
    .r = {0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f},
    .g = {0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f},
    .b = {0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f},
};

constexpr RayColors blue_sky = {
    .r = {0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
    .g = {0.7f, 0.7f, 0.7f, 0.7f, 0.7f, 0.7f, 0.7f, 0.7f},
    .b = {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f},
};

constexpr __m256 reflect_scale = {
    2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f,
};

void scatter_metallic(RayCluster* rays, const HitRecords* hit_rec, RayColors attenuation) {
  // reflect ray direction about the hit record normal
  __m256 reflected_dot = _mm256_mul_ps(rays->dir_x, hit_rec->norm_x);
  reflected_dot = _mm256_fmadd_ps(rays->dir_y, hit_rec->norm_y, reflected_dot);
  reflected_dot = _mm256_fmadd_ps(rays->dir_z, hit_rec->norm_z, reflected_dot);

  __m256 scaled_norm_x = _mm256_mul_ps(hit_rec->norm_x, reflect_scale);
  __m256 scaled_norm_y = _mm256_mul_ps(hit_rec->norm_y, reflect_scale);
  __m256 scaled_norm_z = _mm256_mul_ps(hit_rec->norm_z, reflect_scale);

  rays->dir_x = _mm256_sub_ps(rays->dir_x, scaled_norm_x);
  rays->dir_y = _mm256_sub_ps(rays->dir_y, scaled_norm_y);
  rays->dir_z = _mm256_sub_ps(rays->dir_z, scaled_norm_z);

  // TODO: idek man this is kinda crazy rn we'll see
};

RayColors ray_cluster_colors(const RayCluster* rays, const Sphere* spheres, uint8_t depth) {

  if (depth == 0) {
    return RayColors{
        .r = _mm256_setzero_ps(),
        .g = _mm256_setzero_ps(),
        .b = _mm256_setzero_ps(),
    };
  }

  HitRecords hit_rec = find_sphere_hits(spheres, rays, 100000.f);
  __m256 zeros = _mm256_setzero_ps();
  __m256 res = _mm256_cmp_ps(hit_rec.t, zeros, CMPEQPS);
  if (_mm256_testz_ps(res, res)) {
    return RayColors{
        .r = _mm256_setzero_ps(),
        .g = _mm256_setzero_ps(),
        .b = _mm256_setzero_ps(),
    };
  }
};

int main() {

  RayCluster rays = {
      .dir_x = _mm256_setzero_ps(),
      .dir_y = _mm256_setzero_ps(),
      .dir_z = {-1.f, -1.f, -1.f, -1.f, -1.f, -1.f, -1.f, -1.f},
  };

  // TODO: get rays
  RayColors colors = ray_cluster_colors(&rays, spheres, RAY_DEPTH);

  return 0;
}
