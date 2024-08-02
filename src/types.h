#pragma once
#include <cstdint>
#include <immintrin.h>

struct Vec3 {
  float x;
  float y;
  float z;
};

struct Vec4 {
  float x;
  float y;
  float z;
  float w;
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

struct CharColor {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

struct Color_256 {
  __m256 r;
  __m256 g;
  __m256 b;
};

enum MatType {
  metallic,
  lambertian,
};

struct Material_256 {
  Color_256 atten;
  __m256i type;
};

struct alignas(16) Material {
  Color atten;
  MatType type;
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
  Vec3_256 orig;
};

struct HitRecords {
  Vec3_256 orig;
  Vec3_256 norm;
  Material_256 mat;
  __m256 t;
};
