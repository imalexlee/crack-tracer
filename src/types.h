#pragma once
#include <immintrin.h>

struct Color {
  float r;
  float g;
  float b;
};

struct Vec3_256 {
  __m256 x;
  __m256 y;
  __m256 z;
};

struct Vec3 {
  float x;
  float y;
  float z;
};

struct Colors {
  __m256 r;
  __m256 g;
  __m256 b;
};

struct alignas(16) Sphere {
  Vec3 center;
  float r;
};

struct Material {
  Colors attenuation;
};

struct SphereCluster {
  Vec3_256 center;
  __m256 r;
  __m256i mat_idx;
};

struct RayCluster {
  Vec3_256 dir;
};

struct HitRecords {
  Vec3_256 orig;
  Vec3_256 norm;
  __m256 t;
  __m256i mat_idx;
};
