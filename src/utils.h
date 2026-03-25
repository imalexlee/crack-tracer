#pragma once
#include <cstdio>

#ifndef NDEBUG
#define BREAKPOINT asm("int $3");
#else
#define BREAKPOINT ;
#endif

#ifdef __ARM_NEON
#include <arm_neon.h>

inline void print_vec_128_f32(const float32x4_t vec) {
  printf("%.3f %.3f %.3f %.3f\n", vec[0], vec[1], vec[2], vec[3]);
}

inline void print_vec_128_u8(const uint8x16_t vec) {

    printf("%hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu\n", 
      vec[0], vec[1], vec[2], vec[3], 
      vec[4], vec[5], vec[6], vec[7],
      vec[8], vec[9], vec[10], vec[11],
      vec[12], vec[13], vec[14], vec[15]);
}

#else
#include <immintrin.h>

inline void print_vec_256(const __m256 vec) {

  printf("%.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f\n", vec[0], vec[1], vec[2], vec[3], vec[4],
         vec[5], vec[6], vec[7]);
}

inline void print_vec_128(const __m128 vec) {
  printf("%.3f %.3f %.3f %.3f\n", vec[0], vec[1], vec[2], vec[3]);
}

#endif