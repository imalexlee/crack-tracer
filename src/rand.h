#pragma once
#include "comptime.h"
#include "math.h"

#ifdef __ARM_NEON
#include <arm_neon.h>

class LCGRand {
public:
  [[nodiscard]] inline Vec3_128 random_unit_vec() {
    Vec3_128 rand_vec = rand_vec_in_cube();
    normalize(&rand_vec);
    return rand_vec;
  };

  [[nodiscard]] inline float rand_in_range(float min, float max) {
    float scale = lcg_rand() * rcp_rand_max;
    float f = min + scale * (max - min);
    return f;
  }

  [[nodiscard]] inline float32x4_t rand_in_range_128(float min, float max) {

    uint32x4_t scale_i32 = lcg_rand_128();
    float32x4_t scale = vcvtq_f32_u32(scale_i32);
    float32x4_t rcp_rand_max_vec = vdupq_n_f32(rcp_rand_max);
    scale *= rcp_rand_max_vec;

    float32x4_t min_vec = vdupq_n_f32(min);
    float32x4_t max_vec = vdupq_n_f32(max);
    float32x4_t range = max_vec - min_vec;

    //inverted
    return vmlaq_f32(range, min_vec, scale);
  }

private:
  static inline thread_local uint32x4_t rseed_vec = comptime::init_rseed_arr();
  static inline thread_local uint32_t rseed = 0;
  const uint32x4_t r_a = vdupq_n_u32((uint32_t)11035152453u);
  const uint32x4_t r_b = vdupq_n_u32(12345u);
  const uint32x4_t rand_max_vec = vdupq_n_u32(RAND_MAX);
  static constexpr float rcp_rand_max = 1.f / RAND_MAX;

  [[nodiscard]] inline Vec3_128 rand_vec_in_cube() {
    float min = -1.0;
    float max = 1.0;

    Vec3_128 rand_vec = {
        .x = rand_in_range_128(min, max),
        .y = rand_in_range_128(min, max),
        .z = rand_in_range_128(min, max),
    };

    return rand_vec;
  }

  [[nodiscard]] inline uint32x4_t lcg_rand_128() {

    rseed_vec = vmulq_u32(rseed_vec, r_a);
    rseed_vec = vaddq_u32(rseed_vec, r_b);
    rseed_vec = vandq_u32(rseed_vec, rand_max_vec);

    return rseed_vec;
  };

  // scalar versions of rand generation
  [[nodiscard]] inline int lcg_rand() { return rseed = (rseed * 1103515245u + 12345u) & RAND_MAX; }
};

#else
#include <immintrin.h>

class LCGRand {
public:
  [[nodiscard]] inline Vec3_256 random_unit_vec() {
    Vec3_256 rand_vec = rand_vec_in_cube();
    normalize(&rand_vec);
    return rand_vec;
  };

  [[nodiscard]] inline float rand_in_range(float min, float max) {
    float scale = lcg_rand() * rcp_rand_max;
    float f = min + scale * (max - min);
    return f;
  }

  [[nodiscard]] inline __m256 rand_in_range_256(float min, float max) {
    __m256i scale_i32 = lcg_rand_256();
    __m256 scale = _mm256_cvtepi32_ps(scale_i32);
    __m256 rcp_rand_max_vec = _mm256_broadcast_ss(&rcp_rand_max);
    scale *= rcp_rand_max_vec;

    __m256 min_vec = _mm256_broadcast_ss(&min);
    __m256 max_vec = _mm256_broadcast_ss(&max);
    __m256 range = max_vec - min_vec;

    return _mm256_fmadd_ps(scale, range, min_vec);
  }

private:
  static inline thread_local __m256i rseed_vec = comptime::init_rseed_arr();
  static inline thread_local uint32_t rseed = 0;
  const __m256i r_a = _mm256_set1_epi32((uint32_t)11035152453u);
  const __m256i r_b = _mm256_set1_epi32(12345u);
  const __m256i rand_max_vec = _mm256_set1_epi32(RAND_MAX);
  static constexpr float rcp_rand_max = 1.f / RAND_MAX;

  [[nodiscard]] inline Vec3_256 rand_vec_in_cube() {
    float min = -1.0;
    float max = 1.0;

    Vec3_256 rand_vec = {
        .x = rand_in_range_256(min, max),
        .y = rand_in_range_256(min, max),
        .z = rand_in_range_256(min, max),
    };

    return rand_vec;
  }

  [[nodiscard]] inline __m256i lcg_rand_256() {
    rseed_vec = _mm256_mullo_epi32(rseed_vec, r_a);
    rseed_vec = _mm256_add_epi32(rseed_vec, r_b);
    rseed_vec = _mm256_and_si256(rseed_vec, rand_max_vec);
    return rseed_vec;
  };

  // scalar versions of rand generation
  [[nodiscard]] inline int lcg_rand() { return rseed = (rseed * 1103515245u + 12345u) & RAND_MAX; }
};
#endif