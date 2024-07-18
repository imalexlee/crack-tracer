#include "constants.h"
#include "sphere.h"
#include "types.h"
#include <cmath>
#include <csignal>
#include <cstdint>
#include <cstdio>
#include <immintrin.h>

constexpr Color silver = {
    .r = 0.66f,
    .g = 0.66f,
    .b = 0.66f,
};

constexpr Sphere spheres[SPHERE_NUM] = {{
    .x = 0.f,
    .y = 0.f,
    .z = -1.f,
    .r = 0.5f,
}};

constexpr Colors silver_attenuation = {
    .r = {0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f},
    .g = {0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f},
    .b = {0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f},
};

constexpr Material materials[SPHERE_NUM] = {{
    .attenuation = silver_attenuation,
}};

constexpr Colors blue_sky = {
    .r = {0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
    .g = {0.7f, 0.7f, 0.7f, 0.7f, 0.7f, 0.7f, 0.7f, 0.7f},
    .b = {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f},
};

constexpr __m256 white = {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f};

constexpr __m256 reflect_scale = {
    2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f,
};

void scatter_metallic(RayCluster* rays, const HitRecords* hit_rec) {
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

alignas(32) int mat_indices[8];
Colors ray_cluster_colors(RayCluster* rays, const Sphere* spheres, uint8_t depth) {
  __m256 zeros = _mm256_setzero_ps();
  // will be used to add a sky tint to rays that at some point bounce off into space.
  // if a ray never bounces away (within amount of bounces set by depth), the
  // no_hit_multiplier will be 0.0 and the sky tint will not affect its final color
  __m256 no_hit_multiplier = white;
  HitRecords hit_rec;
  Colors colors{
      .r = white,
      .g = white,
      .b = white,
  };

  for (uint8_t i = 0; i < depth; i++) {

    hit_rec = find_sphere_hits(spheres, rays, INFINITY);

    // break loop if no new hits
    __m256 no_hits = _mm256_cmp_ps(hit_rec.t, zeros, CMPEQPS);
    if (_mm256_testz_ps(no_hits, no_hits)) {
      break;
    }

    // TODO: scatter metallic

    // TODO: multiply current colors by the material attenuation of what
    // was just hit
    //_mm256_store_si256((__m256i*)mat_indices, hit_rec.mat_idx);
  }

  // add a sky tint to rays that went off into space
  __m256 sky_r = _mm256_mul_ps(no_hit_multiplier, blue_sky.r);
  __m256 sky_g = _mm256_mul_ps(no_hit_multiplier, blue_sky.g);
  __m256 sky_b = _mm256_mul_ps(no_hit_multiplier, blue_sky.b);
  colors.r = _mm256_mul_ps(colors.r, sky_r);
  colors.g = _mm256_mul_ps(colors.g, sky_g);
  colors.b = _mm256_mul_ps(colors.b, sky_b);

  return colors;
};

int main() {

  RayCluster rays = {
      .dir_x = _mm256_setzero_ps(),
      .dir_y = _mm256_setzero_ps(),
      .dir_z = {-1.f, -1.f, -1.f, -1.f, -1.f, -1.f, -1.f, -1.f},
  };

  // TODO: get rays
  Colors colors = ray_cluster_colors(&rays, spheres, RAY_DEPTH);

  return 0;
}
