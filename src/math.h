#pragma once
#include "globals.h"
#include "types.h"
#include <cstdint>
#include <cstdio>

#ifdef __ARM_NEON
#include <arm_neon.h>

// OPERATORS

inline static Vec3_128 operator+(const Vec3_128& a, const Vec3_128& b) {
  return Vec3_128{
      .x = vaddq_f32(a.x, b.x),
      .y = vaddq_f32(a.y, b.y),
      .z = vaddq_f32(a.z, b.z),
  };
}

inline static Vec3_128 operator+(const Vec3_128& a, const float32x4_t& b) {
  return Vec3_128{
      .x = vaddq_f32(a.x, b),
      .y = vaddq_f32(a.y, b),
      .z = vaddq_f32(a.z, b),
  };
}

inline static Vec3_128& operator+=(Vec3_128& a, const Vec3_128& b) {
  a.x = vaddq_f32(a.x, b.x);
  a.y = vaddq_f32(a.y, b.y);
  a.z = vaddq_f32(a.z, b.z);
  return a;
}

inline static Vec3_128 operator-(const Vec3_128& a, const Vec3_128& b) {
  return Vec3_128{
      .x = vsubq_f32(a.x, b.x),
      .y = vsubq_f32(a.y, b.y),
      .z = vsubq_f32(a.z, b.z),
  };
}

inline static Vec3_128& operator-=(Vec3_128& a, const Vec3_128& b) {
  a.x = vsubq_f32(a.x, b.x);
  a.y = vsubq_f32(a.y, b.y);
  a.z = vsubq_f32(a.z, b.z);
  return a;
}

// inverse
inline static Vec3_128 operator-(const Vec3_128& a) {
  return Vec3_128{
      .x = vnegq_f32(a.x),
      .y = vnegq_f32(a.y),
      .z = vnegq_f32(a.z),
  };
}

inline static Vec3_128 operator*(const Vec3_128& a, const Vec3_128& b) {
  return Vec3_128{
      .x = vmulq_f32(a.x, b.x),
      .y = vmulq_f32(a.y, b.y),
      .z = vmulq_f32(a.z, b.z),
  };
}

inline static Vec3_128 operator*(const Vec3_128& a, const float32x4_t& b) {
  return Vec3_128{
      .x = vmulq_f32(a.x, b),
      .y = vmulq_f32(a.y, b),
      .z = vmulq_f32(a.z, b),
  };
}

inline static Vec3_128& operator*=(Vec3_128& a, const Vec3_128& b) {
  a.x = vmulq_f32(a.x, b.x);
  a.y = vmulq_f32(a.y, b.y);
  a.z = vmulq_f32(a.z, b.z);
  return a;
}

inline static Vec3_128& operator*=(Vec3_128& a, const float32x4_t& b) {
  a.x = vmulq_f32(a.x, b);
  a.y = vmulq_f32(a.y, b);
  a.z = vmulq_f32(a.z, b);
  return a;
}

inline static Vec3_128 operator/(const Vec3_128& a, const Vec3_128& b) {
  return Vec3_128{
      .x = vdivq_f32(a.x, b.x),
      .y = vdivq_f32(a.y, b.y),
      .z = vdivq_f32(a.z, b.z),
  };
}

inline static Vec3_128& operator/=(Vec3_128& a, const float32x4_t& b) {
  //accuracy issue with reciprocal
  float32x4_t rcp_b = vdivq_f32(global::ones, b);

  a.x = vmulq_f32(a.x, rcp_b);
  a.y = vmulq_f32(a.y, rcp_b);
  a.z = vmulq_f32(a.z, rcp_b);
  return a;
}

inline static Vec3_128 operator&(const Vec3_128& a, const uint32x4_t& b) {

  uint32x4_t x = vandq_u32(vreinterpretq_u32_f32(a.x), b);
  uint32x4_t y = vandq_u32(vreinterpretq_u32_f32(a.y), b);
  uint32x4_t z = vandq_u32(vreinterpretq_u32_f32(a.z), b);

  return Vec3_128{
      .x = vreinterpretq_f32_u32(x),
      .y = vreinterpretq_f32_u32(y),
      .z = vreinterpretq_f32_u32(z),
  };
}

inline static Vec3_128& operator&=(Vec3_128& a, const uint32x4_t& b) {

  uint32x4_t x = vandq_u32(vreinterpretq_u32_f32(a.x), b);
  uint32x4_t y = vandq_u32(vreinterpretq_u32_f32(a.y), b);
  uint32x4_t z = vandq_u32(vreinterpretq_u32_f32(a.z), b);

  a.x = vreinterpretq_f32_u32(x);
  a.y = vreinterpretq_f32_u32(y);
  a.z = vreinterpretq_f32_u32(z);
  return a;
}

inline static float32x4_t rsqrt(float32x4_t x)
{
  float32x4_t rsqrt_estimate = vrsqrteq_f32(x);

  //Newton-Raphson 16 bit
  float32x4_t step1 = vrsqrtsq_f32(vmulq_f32(rsqrt_estimate, rsqrt_estimate), x);
  return vmulq_f32(rsqrt_estimate, step1);
}

[[nodiscard]] inline static float32x4_t dot(const Vec3_128* a, const Vec3_128* b) {
  float32x4_t dot = vmulq_f32(a->x, b->x);

  //inverted 
  dot = vmlaq_f32(dot, a->y, b->y);
  return vmlaq_f32(dot, a->z, b->z);
}

// reflect a ray about the axis
// v = v - 2*dot(v,n)*n;
[[nodiscard]] inline static Vec3_128 reflect(const Vec3_128* ray_dir, const Vec3_128* axis) {
  float32x4_t reflect_scale = vdupq_n_f32(2.f);

  return *ray_dir - *axis * dot(ray_dir, axis) * reflect_scale;
}

[[nodiscard]] inline static float32x4_t abs_128(float32x4_t vec) {
  return vabsq_f32(vec);
}

[[nodiscard]] inline static Vec3_128 refract(const Vec3_128* ray_dir, const Vec3_128* norm,
                                             float32x4_t ratio) {                                     
  Vec3_128 inverted_ray_dir = -*ray_dir;
  float32x4_t cos_theta = vminq_f32(dot(&inverted_ray_dir, norm), global::ones);

  //inverted
  Vec3_128 r_out_perp = {
      .x = vmlaq_f32(ray_dir->x, norm->x, cos_theta),
      .y = vmlaq_f32(ray_dir->y, norm->y, cos_theta),
      .z = vmlaq_f32(ray_dir->z, norm->z, cos_theta),
  };
  r_out_perp *= ratio;

  float32x4_t r_out_parallel_scale = global::ones - dot(&r_out_perp, &r_out_perp);

  r_out_parallel_scale = abs_128(r_out_parallel_scale);

  // square then negate
  float32x4_t parallel_scale_rsqrt = rsqrt(r_out_parallel_scale);
  r_out_parallel_scale *= -parallel_scale_rsqrt;

  //inverted
  return Vec3_128{
      .x = vmlaq_f32(r_out_perp.x, norm->x, r_out_parallel_scale),
      .y = vmlaq_f32(r_out_perp.y, norm->y, r_out_parallel_scale),
      .z = vmlaq_f32(r_out_perp.z, norm->z, r_out_parallel_scale),
  };
}

inline static void normalize(Vec3_128* vec) {
  float32x4_t vec_len_2 = dot(vec, vec);
  float32x4_t recip_len = rsqrt(vec_len_2);//vrsqrteq_f32(vec_len_2);

  *vec *= recip_len;
}

inline static Vec3_128 broadcast_vec(const Vec3* vec) {
  return Vec3_128{
      .x = vdupq_n_f32(vec->x),
      .y = vdupq_n_f32(vec->y),
      .z = vdupq_n_f32(vec->z),
  };
}

inline static Vec3_128 blend_vec128(const Vec3_128* a, const Vec3_128* b, uint32x4_t mask) {
  return Vec3_128{
      //inverted
      .x = vbslq_f32(mask, b->x, a->x),
      .y = vbslq_f32(mask, b->y, a->y),
      .z = vbslq_f32(mask, b->z, a->z),
  };
}

inline static int testz_128(uint32x4_t a) {
  return vmaxvq_u32(a) == 0;
}

inline static uint32_t f_to_i(float f_val) {
  f_val += 1 << 23;
  return ((uint32_t)f_val) & 0x007FFFFF;
}

#else
#include <immintrin.h>

// OPERATORS

inline static Vec3_256 operator+(const Vec3_256& a, const Vec3_256& b) {
  return Vec3_256{
      .x = _mm256_add_ps(a.x, b.x),
      .y = _mm256_add_ps(a.y, b.y),
      .z = _mm256_add_ps(a.z, b.z),
  };
}

inline static Vec3_256 operator+(const Vec3_256& a, const __m256& b) {
  return Vec3_256{
      .x = _mm256_add_ps(a.x, b),
      .y = _mm256_add_ps(a.y, b),
      .z = _mm256_add_ps(a.z, b),
  };
}

inline static Vec3_256& operator+=(Vec3_256& a, const Vec3_256& b) {
  a.x = _mm256_add_ps(a.x, b.x);
  a.y = _mm256_add_ps(a.y, b.y);
  a.z = _mm256_add_ps(a.z, b.z);
  return a;
}

inline static Vec3_256 operator-(const Vec3_256& a, const Vec3_256& b) {
  return Vec3_256{
      .x = _mm256_sub_ps(a.x, b.x),
      .y = _mm256_sub_ps(a.y, b.y),
      .z = _mm256_sub_ps(a.z, b.z),
  };
}

inline static Vec3_256& operator-=(Vec3_256& a, const Vec3_256& b) {
  a.x = _mm256_sub_ps(a.x, b.x);
  a.y = _mm256_sub_ps(a.y, b.y);
  a.z = _mm256_sub_ps(a.z, b.z);
  return a;
}

// inverse
inline static Vec3_256 operator-(const Vec3_256& a) {
  // -1
  __m256 invert = _mm256_sub_ps(_mm256_setzero_ps(), global::white);
  return Vec3_256{
      .x = _mm256_mul_ps(a.x, invert),
      .y = _mm256_mul_ps(a.y, invert),
      .z = _mm256_mul_ps(a.z, invert),
  };
}

inline static Vec3_256 operator*(const Vec3_256& a, const Vec3_256& b) {
  return Vec3_256{
      .x = _mm256_mul_ps(a.x, b.x),
      .y = _mm256_mul_ps(a.y, b.y),
      .z = _mm256_mul_ps(a.z, b.z),
  };
}

inline static Vec3_256 operator*(const Vec3_256& a, const __m256& b) {
  return Vec3_256{
      .x = _mm256_mul_ps(a.x, b),
      .y = _mm256_mul_ps(a.y, b),
      .z = _mm256_mul_ps(a.z, b),
  };
}

inline static Vec3_256& operator*=(Vec3_256& a, const Vec3_256& b) {
  a.x = _mm256_mul_ps(a.x, b.x);
  a.y = _mm256_mul_ps(a.y, b.y);
  a.z = _mm256_mul_ps(a.z, b.z);
  return a;
}

inline static Vec3_256& operator*=(Vec3_256& a, const __m256& b) {
  a.x = _mm256_mul_ps(a.x, b);
  a.y = _mm256_mul_ps(a.y, b);
  a.z = _mm256_mul_ps(a.z, b);
  return a;
}

inline static Vec3_256 operator/(const Vec3_256& a, const Vec3_256& b) {

  Vec3_256 rcp_b = {
      .x = _mm256_rcp_ps(b.x),
      .y = _mm256_rcp_ps(b.y),
      .z = _mm256_rcp_ps(b.z),
  };

  return Vec3_256{
      .x = _mm256_mul_ps(a.x, rcp_b.x),
      .y = _mm256_mul_ps(a.y, rcp_b.y),
      .z = _mm256_mul_ps(a.z, rcp_b.z),
  };
}

inline static Vec3_256& operator/=(Vec3_256& a, const __m256& b) {

  __m256 rcp_b = _mm256_rcp_ps(b);

  a.x = _mm256_mul_ps(a.x, rcp_b);
  a.y = _mm256_mul_ps(a.y, rcp_b);
  a.z = _mm256_mul_ps(a.z, rcp_b);
  return a;
}

inline static Vec3_256 operator&(const Vec3_256& a, const __m256& b) {
  return Vec3_256{
      .x = _mm256_and_ps(a.x, b),
      .y = _mm256_and_ps(a.y, b),
      .z = _mm256_and_ps(a.z, b),
  };
}

inline static Vec3_256& operator&=(Vec3_256& a, const __m256& b) {
  a.x = _mm256_and_ps(a.x, b);
  a.y = _mm256_and_ps(a.y, b);
  a.z = _mm256_and_ps(a.z, b);
  return a;
}

[[nodiscard]] inline static __m256 dot(const Vec3_256* a, const Vec3_256* b) {
  __m256 dot = _mm256_mul_ps(a->x, b->x);
  dot = _mm256_fmadd_ps(a->y, b->y, dot);
  return _mm256_fmadd_ps(a->z, b->z, dot);
}

// reflect a ray about the axis
// v = v - 2*dot(v,n)*n;
[[nodiscard]] inline static Vec3_256 reflect(const Vec3_256* ray_dir, const Vec3_256* axis) {
  constexpr __m256 reflect_scale = {
      2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f,
  };
  return *ray_dir - *axis * dot(ray_dir, axis) * reflect_scale;
}

[[nodiscard]] inline static __m256 abs_256(__m256 vec) {
  __m256i sign_mask = _mm256_srli_epi32((__m256i)global::all_set, 1);
  return _mm256_and_ps(vec, (__m256)sign_mask);
}

[[nodiscard]] inline static Vec3_256 refract(const Vec3_256* ray_dir, const Vec3_256* norm,
                                             __m256 ratio) {

  Vec3_256 inverted_ray_dir = -*ray_dir;
  __m256 cos_theta = _mm256_min_ps(dot(&inverted_ray_dir, norm), global::white);

  Vec3_256 r_out_perp = {
      .x = _mm256_fmadd_ps(cos_theta, norm->x, ray_dir->x),
      .y = _mm256_fmadd_ps(cos_theta, norm->y, ray_dir->y),
      .z = _mm256_fmadd_ps(cos_theta, norm->z, ray_dir->z),
  };
  r_out_perp *= ratio;

  __m256 r_out_parallel_scale = global::white - dot(&r_out_perp, &r_out_perp);

  r_out_parallel_scale = abs_256(r_out_parallel_scale);

  // square then negate
  __m256 parallel_scale_rsqrt = _mm256_rsqrt_ps(r_out_parallel_scale);
  r_out_parallel_scale *= -parallel_scale_rsqrt;

  return Vec3_256{
      .x = _mm256_fmadd_ps(r_out_parallel_scale, norm->x, r_out_perp.x),
      .y = _mm256_fmadd_ps(r_out_parallel_scale, norm->y, r_out_perp.y),
      .z = _mm256_fmadd_ps(r_out_parallel_scale, norm->z, r_out_perp.z),
  };
}

inline static void normalize(Vec3_256* vec) {
  __m256 vec_len_2 = dot(vec, vec);
  __m256 recip_len = _mm256_rsqrt_ps(vec_len_2);

  *vec *= recip_len;
}

inline static Vec3_256 broadcast_vec(const Vec3* vec) {
  return Vec3_256{
      .x = _mm256_broadcast_ss(&vec->x),
      .y = _mm256_broadcast_ss(&vec->y),
      .z = _mm256_broadcast_ss(&vec->z),
  };
}

inline static Vec3_256 blend_vec256(const Vec3_256* a, const Vec3_256* b, __m256 mask) {
  return Vec3_256{
      .x = _mm256_blendv_ps(a->x, b->x, mask),
      .y = _mm256_blendv_ps(a->y, b->y, mask),
      .z = _mm256_blendv_ps(a->z, b->z, mask),
  };
}

inline static uint32_t f_to_i(float f_val) {
  f_val += 1 << 23;
  return ((uint32_t)f_val) & 0x007FFFFF;
}

#endif