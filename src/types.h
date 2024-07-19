#pragma once
#include <immintrin.h>

struct Vec3 {
  float x;
  float y;
  float z;
};

struct Vec3_256 {
  __m256 x;
  __m256 y;
  __m256 z;
};

struct Color {
  float r;
  float g;
  float b;
};

struct Color_256 {
  __m256 r;
  __m256 g;
  __m256 b;
};

struct alignas(16) Material {
  Color atten;
};

struct Material_256 {
  Color_256 atten;
};

struct alignas(32) Sphere {
  Vec3 center;
  Material mat;
  float r;
};

struct SphereCluster {
  Vec3_256 center;
  Material_256 mat;
  __m256 r;
};

struct RayCluster {
  Vec3_256 dir;
};

struct HitRecords {
  Vec3_256 orig;
  Vec3_256 norm;
  Material_256 mat;
  __m256 t;
};
