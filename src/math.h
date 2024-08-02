#pragma once
#include "globals.h"
#include "types.h"
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <immintrin.h>
#include <xmmintrin.h>

[[nodiscard]] inline static __m256 dot(const Vec3_256* a, const Vec3_256* b) {
  __m256 dot = _mm256_mul_ps(a->x, b->x);
  dot = _mm256_fmadd_ps(a->y, b->y, dot);
  return _mm256_fmadd_ps(a->z, b->z, dot);
}

// reflect a ray about the axis
// v = v - 2*dot(v,n)*n;
inline static void reflect(Vec3_256* reflect_ray, const Vec3_256* axis) {
  constexpr static __m256 reflect_scale = {
      2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f,
  };

  __m256 reflected_dot = dot(reflect_ray, axis);

  reflected_dot = _mm256_mul_ps(reflected_dot, reflect_scale);

  __m256 scaled_norm_x = _mm256_mul_ps(axis->x, reflected_dot);
  __m256 scaled_norm_y = _mm256_mul_ps(axis->y, reflected_dot);
  __m256 scaled_norm_z = _mm256_mul_ps(axis->z, reflected_dot);

  reflect_ray->x = _mm256_sub_ps(reflect_ray->x, scaled_norm_x);
  reflect_ray->y = _mm256_sub_ps(reflect_ray->y, scaled_norm_y);
  reflect_ray->z = _mm256_sub_ps(reflect_ray->z, scaled_norm_z);
}

inline static void normalize(Vec3_256* vec) {
  __m256 vec_len_2 = dot(vec, vec);
  __m256 recip_len = _mm256_rsqrt_ps(vec_len_2);

  vec->x = _mm256_mul_ps(vec->x, recip_len);
  vec->y = _mm256_mul_ps(vec->y, recip_len);
  vec->z = _mm256_mul_ps(vec->z, recip_len);
}

[[nodiscard]] inline static int lcg_rand() {
  return global::rseed = (global::rseed * 11035152453 + 12345) & RAND_MAX;
}

[[nodiscard]] inline static float rand_in_range(float min, float max) {
  float scale = lcg_rand() / (float)RAND_MAX;
  float f = min + scale * (max - min);
  return f;
}

// finds random vectors in unit cube
[[nodiscard]] inline static Vec3_256 rand_vec_in_cube() {
  float min = -1.0;
  float max = 1.0;

  alignas(32) float x_arr[8];
  alignas(32) float y_arr[8];
  alignas(32) float z_arr[8];

  for (int i = 0; i < 8; i++) {
    x_arr[i] = rand_in_range(min, max);
    y_arr[i] = rand_in_range(min, max);
    z_arr[i] = rand_in_range(min, max);
  }

  Vec3_256 rand_vec = {
      .x = _mm256_load_ps(x_arr),
      .y = _mm256_load_ps(y_arr),
      .z = _mm256_load_ps(z_arr),
  };

  return rand_vec;
}

[[nodiscard]] inline static Vec3_256 random_unit_vec() {
  Vec3_256 rand_vec = rand_vec_in_cube();
  normalize(&rand_vec);
  return rand_vec;
};

inline static uint32_t f_to_i(float f_val) {
  f_val += 1 << 23;
  return ((uint32_t)f_val) & 0x007FFFFF;
}
