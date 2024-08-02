#pragma once
#include "math.h"
#include "rand.h"
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

alignas(32) static const int emissive_types[8] = {
    MatType::emissive, MatType::emissive, MatType::emissive, MatType::emissive,
    MatType::emissive, MatType::emissive, MatType::emissive, MatType::emissive,
};

static LCGRand lcg_rand;

inline static void scatter_metallic(RayCluster* rays, const HitRecords* hit_rec) {
  reflect(&rays->dir, &hit_rec->norm);
  normalize(&rays->dir);
};

inline static void scatter_lambertian(RayCluster* rays, const HitRecords* hit_rec) {
  Vec3_256 rand_vec = lcg_rand.random_unit_vec();

  rays->dir.x = _mm256_add_ps(rand_vec.x, hit_rec->norm.x);
  rays->dir.y = _mm256_add_ps(rand_vec.y, hit_rec->norm.y);
  rays->dir.z = _mm256_add_ps(rand_vec.z, hit_rec->norm.z);
}

inline static void scatter_emissive(RayCluster* rays, [[maybe_unused]] const HitRecords* hit_rec) {
  rays->dir.x = _mm256_setzero_ps();
  rays->dir.y = _mm256_setzero_ps();
  rays->dir.z = _mm256_setzero_ps();
}

inline static void scatter(RayCluster* rays, const HitRecords* hit_rec) {
  __m256i metallic_type = _mm256_load_si256((__m256i*)metallic_types);
  __m256i lambertian_type = _mm256_load_si256((__m256i*)lambertian_types);
  __m256i emissive_type = _mm256_load_si256((__m256i*)emissive_types);

  __m256i metallic_loc = _mm256_cmpeq_epi32(hit_rec->mat.type, metallic_type);
  __m256i lambertian_loc = _mm256_cmpeq_epi32(hit_rec->mat.type, lambertian_type);
  __m256i emissive_loc = _mm256_cmpeq_epi32(hit_rec->mat.type, emissive_type);

  if (!_mm256_testz_si256(metallic_loc, metallic_loc)) {
    RayCluster metallic_rays = {
        .dir = rays->dir,
        .orig = hit_rec->orig,
    };
    scatter_metallic(&metallic_rays, hit_rec);

    rays->dir.x = _mm256_blendv_ps(rays->dir.x, metallic_rays.dir.x, (__m256)metallic_loc);
    rays->dir.y = _mm256_blendv_ps(rays->dir.y, metallic_rays.dir.y, (__m256)metallic_loc);
    rays->dir.z = _mm256_blendv_ps(rays->dir.z, metallic_rays.dir.z, (__m256)metallic_loc);

    rays->orig.x = _mm256_blendv_ps(rays->orig.x, metallic_rays.orig.x, (__m256)metallic_loc);
    rays->orig.y = _mm256_blendv_ps(rays->orig.y, metallic_rays.orig.y, (__m256)metallic_loc);
    rays->orig.z = _mm256_blendv_ps(rays->orig.z, metallic_rays.orig.z, (__m256)metallic_loc);
  }
  if (!_mm256_testz_si256(lambertian_loc, lambertian_loc)) {
    RayCluster lambertian_rays = {
        .dir = rays->dir,
        .orig = hit_rec->orig,
    };
    scatter_lambertian(&lambertian_rays, hit_rec);

    rays->dir.x = _mm256_blendv_ps(rays->dir.x, lambertian_rays.dir.x, (__m256)lambertian_loc);
    rays->dir.y = _mm256_blendv_ps(rays->dir.y, lambertian_rays.dir.y, (__m256)lambertian_loc);
    rays->dir.z = _mm256_blendv_ps(rays->dir.z, lambertian_rays.dir.z, (__m256)lambertian_loc);

    rays->orig.x = _mm256_blendv_ps(rays->orig.x, lambertian_rays.orig.x, (__m256)lambertian_loc);
    rays->orig.y = _mm256_blendv_ps(rays->orig.y, lambertian_rays.orig.y, (__m256)lambertian_loc);
    rays->orig.z = _mm256_blendv_ps(rays->orig.z, lambertian_rays.orig.z, (__m256)lambertian_loc);
  }
  if (!_mm256_testz_si256(emissive_loc, emissive_loc)) {
    RayCluster emissive_rays = {
        .dir = rays->dir,
        .orig = hit_rec->orig,
    };
    scatter_emissive(&emissive_rays, hit_rec);

    rays->orig.x = _mm256_blendv_ps(rays->orig.x, emissive_rays.orig.x, (__m256)emissive_loc);
    rays->orig.y = _mm256_blendv_ps(rays->orig.y, emissive_rays.orig.y, (__m256)emissive_loc);
    rays->orig.z = _mm256_blendv_ps(rays->orig.z, emissive_rays.orig.z, (__m256)emissive_loc);
  }
}
