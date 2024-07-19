#pragma once
#include "constants.h"
#include <immintrin.h>
namespace global {
  const static __m256 all_set = _mm256_cmp_ps(_mm256_setzero_ps(), _mm256_setzero_ps(), CMPEQPS);
  constexpr static __m256 white = {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f};
} // namespace global
