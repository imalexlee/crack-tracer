#pragma once
#include <cstdio>
#include <immintrin.h>

inline void print_vec_256(__m256 vec) {

  printf("%f %f %f %f %f %f %f %f\n", vec[0], vec[1], vec[2], vec[3], vec[4], vec[5], vec[6],
         vec[7]);
}

inline void print_vec_128(__m128 vec) { printf("%f %f %f %f\n", vec[0], vec[1], vec[2], vec[3]); }
