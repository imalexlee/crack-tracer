#pragma once
#include "globals.h"
#include "types.h"
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
inline static void reflect(Vec3_256* ray_dir, const Vec3_256* axis) {
  constexpr static __m256 reflect_scale = {
      2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f,
  };

  __m256 reflected_dot = dot(ray_dir, axis);

  reflected_dot = _mm256_mul_ps(reflected_dot, reflect_scale);

  __m256 scaled_norm_x = _mm256_mul_ps(axis->x, reflected_dot);
  __m256 scaled_norm_y = _mm256_mul_ps(axis->y, reflected_dot);
  __m256 scaled_norm_z = _mm256_mul_ps(axis->z, reflected_dot);

  ray_dir->x = _mm256_sub_ps(ray_dir->x, scaled_norm_x);
  ray_dir->y = _mm256_sub_ps(ray_dir->y, scaled_norm_y);
  ray_dir->z = _mm256_sub_ps(ray_dir->z, scaled_norm_z);
}

inline static void refract(Vec3_256* ray_dir, const Vec3_256* norm, __m256 ratio) {
  __m256 invert = _mm256_sub_ps(_mm256_setzero_ps(), global::white);
  Vec3_256 inverted_ray_dir = {
      .x = _mm256_mul_ps(ray_dir->x, invert),
      .y = _mm256_mul_ps(ray_dir->y, invert),
      .z = _mm256_mul_ps(ray_dir->z, invert),
  };
  __m256 cos_theta = dot(&inverted_ray_dir, norm);
  cos_theta = _mm256_min_ps(cos_theta, global::white);

  Vec3_256 r_out_perp = {
      .x = _mm256_fmadd_ps(cos_theta, norm->x, ray_dir->x),
      .y = _mm256_fmadd_ps(cos_theta, norm->y, ray_dir->y),
      .z = _mm256_fmadd_ps(cos_theta, norm->z, ray_dir->z),
  };
  r_out_perp.x = _mm256_mul_ps(ratio, r_out_perp.x);
  r_out_perp.y = _mm256_mul_ps(ratio, r_out_perp.y);
  r_out_perp.z = _mm256_mul_ps(ratio, r_out_perp.z);

  __m256 r_out_parallel_scale = dot(&r_out_perp, &r_out_perp);
  r_out_parallel_scale = _mm256_sub_ps(global::white, r_out_parallel_scale);

  // absolute value
  __m256i sign_mask = _mm256_srli_epi32((__m256i)global::all_set, 1);
  r_out_parallel_scale = _mm256_and_ps(r_out_parallel_scale, (__m256)sign_mask);
  __m256 parallel_scale_rsqrt = _mm256_rsqrt_ps(r_out_parallel_scale);
  r_out_parallel_scale = _mm256_mul_ps(r_out_parallel_scale, parallel_scale_rsqrt);
  r_out_parallel_scale = _mm256_mul_ps(r_out_parallel_scale, invert);

  ray_dir->x = _mm256_fmadd_ps(r_out_parallel_scale, norm->x, r_out_perp.x);
  ray_dir->y = _mm256_fmadd_ps(r_out_parallel_scale, norm->y, r_out_perp.y);
  ray_dir->z = _mm256_fmadd_ps(r_out_parallel_scale, norm->z, r_out_perp.z);
}

inline static void normalize(Vec3_256* vec) {
  __m256 vec_len_2 = dot(vec, vec);
  __m256 recip_len = _mm256_rsqrt_ps(vec_len_2);

  vec->x = _mm256_mul_ps(vec->x, recip_len);
  vec->y = _mm256_mul_ps(vec->y, recip_len);
  vec->z = _mm256_mul_ps(vec->z, recip_len);
}

inline static uint32_t f_to_i(float f_val) {
  f_val += 1 << 23;
  return ((uint32_t)f_val) & 0x007FFFFF;
}
