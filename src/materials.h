#pragma once
#include "math.h"
#include "types.h"
#include <immintrin.h>

alignas(32) static const int metallic_types[8] = {
    MatType::metallic, MatType::metallic, MatType::metallic, MatType::metallic,
    MatType::metallic, MatType::metallic, MatType::metallic, MatType::metallic,
};

alignas(32) static const int lambertian_types[8] = {
    MatType::lambertian, MatType::lambertian, MatType::lambertian, MatType::lambertian,
    MatType::lambertian, MatType::lambertian, MatType::lambertian, MatType::lambertian,
};

inline static void scatter_metallic(RayCluster* rays, const HitRecords* hit_rec) {
  reflect(&rays->dir, &hit_rec->norm);
  normalize(&rays->dir);
};

inline static void scatter_lambertian(RayCluster* rays, const HitRecords* hit_rec) {
  // TODO
  scatter_metallic(rays, hit_rec);
}

inline static void scatter(RayCluster* rays, const HitRecords* hit_rec) {
  __m256i metallic_type = _mm256_load_si256((__m256i*)metallic_types);
  __m256i lambertian_type = _mm256_load_si256((__m256i*)lambertian_types);

  __m256i metallic_loc = _mm256_cmpeq_epi32(hit_rec->mat.type, metallic_type);
  __m256i lambertian_loc = _mm256_cmpeq_epi32(hit_rec->mat.type, lambertian_type);

  if (!_mm256_testz_si256(metallic_loc, metallic_loc)) {
    RayCluster metallic_rays = {
        .dir = {.x = rays->dir.x, .y = rays->dir.y, .z = rays->dir.z},
    };
    scatter_metallic(&metallic_rays, hit_rec);

    rays->dir.x = _mm256_blendv_ps(rays->dir.x, metallic_rays.dir.x, (__m256)metallic_loc);
    rays->dir.y = _mm256_blendv_ps(rays->dir.y, metallic_rays.dir.y, (__m256)metallic_loc);
    rays->dir.z = _mm256_blendv_ps(rays->dir.z, metallic_rays.dir.z, (__m256)metallic_loc);
  }
  if (!_mm256_testz_si256(lambertian_loc, lambertian_loc)) {
    RayCluster lambertian_rays = {
        .dir = {.x = rays->dir.x, .y = rays->dir.y, .z = rays->dir.z},
    };
    scatter_lambertian(&lambertian_rays, hit_rec);

    rays->dir.x = _mm256_blendv_ps(rays->dir.x, lambertian_rays.dir.x, (__m256)lambertian_loc);
    rays->dir.y = _mm256_blendv_ps(rays->dir.y, lambertian_rays.dir.y, (__m256)lambertian_loc);
    rays->dir.z = _mm256_blendv_ps(rays->dir.z, lambertian_rays.dir.z, (__m256)lambertian_loc);
  }
}
