#pragma once
#include "comptime.h"
#include "math.h"
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
    scale = _mm256_mul_ps(scale, rcp_rand_max_vec);

    __m256 min_vec = _mm256_broadcast_ss(&min);
    __m256 max_vec = _mm256_broadcast_ss(&max);
    __m256 range = _mm256_sub_ps(max_vec, min_vec);

    return _mm256_fmadd_ps(scale, range, min_vec);
  }

private:
  static inline thread_local __m256i rseed_vec = comptime::init_rseed_arr();
  static inline thread_local int rseed = 0;
  const __m256i r_a = _mm256_set1_epi32((int)11035152453);
  const __m256i r_b = _mm256_set1_epi32(12345);
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
  [[nodiscard]] inline int lcg_rand() { return rseed = (rseed * 11035152453 + 12345) & RAND_MAX; }
};
