#pragma once
#include "globals.h"
#include "math.h"
#include "rand.h"
#include "types.h"
#include <cmath>
#include <cstdlib>

constexpr Color silver = {.x = 0.5f, .y = 0.5f, .z = 0.5f};
constexpr Color grey = {.x = 0.5f, .y = 0.5f, .z = 0.5f};
constexpr Color white = {.x = 1.f, .y = 1.f, .z = 1.f};
constexpr Color red = {.x = 0.90f, .y = 0.20f, .z = 0.20f};
constexpr Color gold = {.x = 0.90f, .y = 0.75f, .z = 0.54f};
constexpr Color copper = {.x = 0.59f, .y = 0.34f, .z = 0.29f};
constexpr Color green = {.x = 0.f, .y = 1.f, .z = 0.f};
constexpr Color moon = {.x = 100.f, .y = 100.f, .z = 100.f};

constexpr Material silver_metallic = {.atten = silver, .type = MatType::metallic};
constexpr Material red_metallic = {.atten = red, .type = MatType::metallic};
constexpr Material gold_metallic = {.atten = gold, .type = MatType::metallic};
constexpr Material copper_metallic = {.atten = copper, .type = MatType::metallic};
constexpr Material green_metallic = {.atten = green, .type = MatType::metallic};

constexpr Material silver_lambertian = {.atten = silver, .type = MatType::lambertian};
constexpr Material red_lambertian = {.atten = red, .type = MatType::lambertian};
constexpr Material gold_lambertian = {.atten = gold, .type = MatType::lambertian};
constexpr Material star_lambertian = {.atten = moon, .type = MatType::lambertian};
constexpr Material grey_lambertian = {.atten = grey, .type = MatType::lambertian};

constexpr Material glass = {.atten = white, .type = MatType::dielectric};

#ifdef __ARM_NEON
#include <arm_neon.h>

alignas(32) static const uint32_t metallic_types[4] = {
    MatType::metallic, MatType::metallic, MatType::metallic, MatType::metallic
};

alignas(32) static const uint32_t lambertian_types[4] = {
    MatType::lambertian, MatType::lambertian, MatType::lambertian, MatType::lambertian
};

alignas(32) static const uint32_t dielectric_types[4] = {
    MatType::dielectric, MatType::dielectric, MatType::dielectric, MatType::dielectric
};

static LCGRand lcg_rand;
inline static void scatter_metallic(RayCluster* rays, const HitRecords* hit_rec) {
  Vec3_128 reflected = reflect(&rays->dir, &hit_rec->norm);
  normalize(&reflected);

  float32x4_t dp = dot(&reflected, &hit_rec->norm);
  uint32x4_t greater_than_zero = vcgtq_f32(dp, global::zeros);

  rays->dir = reflected & greater_than_zero;
};

[[nodiscard]] inline static uint32x4_t near_zero(const Vec3_128* vec) {

  uint32x4_t near_x =  vcltq_f32(abs_128(vec->x), global::t_min_vec);
  uint32x4_t near_y =  vcltq_f32(abs_128(vec->y), global::t_min_vec);
  uint32x4_t near_z =  vcltq_f32(abs_128(vec->z), global::t_min_vec);

  return vandq_u32(near_x, vandq_u32(near_y, near_z));
};

inline static void scatter_lambertian(RayCluster* rays, const HitRecords* hit_rec) {
  Vec3_128 rand_vec = lcg_rand.random_unit_vec();
  Vec3_128 scatter_dir = rand_vec + hit_rec->norm;

  //  rays->dir = blend_vec256(&scatter_dir, &hit_rec->norm, near_zero(&scatter_dir));
  rays->dir = scatter_dir;
}

[[nodiscard]] inline static float32x4_t reflectance(float32x4_t cos, float32x4_t ref_idx) {
  float32x4_t ref_low = global::white - ref_idx;
  float32x4_t ref_high = global::white + ref_idx;

  ref_high = vrecpeq_f32(ref_high);
  float32x4_t ref = ref_low * ref_high;
  ref *= ref;

  float32x4_t cos_sub = global::white - cos;
  // cos_sub^5
  float32x4_t cos_5 = cos_sub * cos_sub;
  cos_5 *= cos_sub;
  cos_5 *= cos_sub;
  cos_5 *= cos_sub;

  float32x4_t ref_sub = global::white - ref;

  //inverted
  return vmlaq_f32(ref, ref_sub, cos_5);
}

inline static void scatter_dielectric(RayCluster* rays, const HitRecords* hit_rec) {

  float32x4_t ri = vbslq_f32(hit_rec->front_face, global::ir_vec, global::rcp_ir_vec);
  Vec3_128 unit_dir = rays->dir;
  normalize(&unit_dir);
  
  Vec3_128 inverse_unit_dir = -unit_dir;

  float32x4_t cos_theta = dot(&inverse_unit_dir, &hit_rec->norm);
  cos_theta = vminq_f32(cos_theta, global::white);

  float32x4_t sin_theta = vsqrtq_f32(global::white - cos_theta * cos_theta);

  uint32x4_t can_refract = vcleq_f32(ri * sin_theta, global::white);

  float32x4_t ref = reflectance(cos_theta, ri);
  float32x4_t rand_vec = lcg_rand.rand_in_range_128(0.f, 1.f);
  uint32x4_t low_reflectance_loc = vcleq_f32(ref, rand_vec);
  uint32x4_t refraction_loc = vandq_u32(can_refract, low_reflectance_loc);
  uint32x4_t reflection_loc = veorq_u32(refraction_loc, global::all_set);

  if (!testz_128(refraction_loc)) {
    Vec3_128 refract_dir = refract(&unit_dir, &hit_rec->norm, ri);
    rays->dir = blend_vec128(&rays->dir, &refract_dir, refraction_loc);
  }
  if (!testz_128(reflection_loc)) {
    Vec3_128 reflect_dir = reflect(&unit_dir, &hit_rec->norm);
    reflection_loc = vandq_u32(reflection_loc, hit_rec->front_face);
    rays->dir = blend_vec128(&rays->dir, &reflect_dir, reflection_loc);
  }
}

inline static void scatter(RayCluster* rays, const HitRecords* hit_rec) {
  uint32x4_t metallic_type = vld1q_u32(metallic_types);
  uint32x4_t lambertian_type = vld1q_u32(lambertian_types);
  uint32x4_t dielectric_type = vld1q_u32(dielectric_types);

  uint32x4_t metallic_loc = vceqq_u32(hit_rec->mat.type, metallic_type);
  uint32x4_t lambertian_loc = vceqq_u32(hit_rec->mat.type, lambertian_type);
  uint32x4_t dielectric_loc = vceqq_u32(hit_rec->mat.type, dielectric_type);

  if (!testz_128(metallic_loc)) {
    RayCluster metallic_rays = {
        .dir = rays->dir,
        .orig = hit_rec->orig,
    };
    scatter_metallic(&metallic_rays, hit_rec);

    rays->dir = blend_vec128(&rays->dir, &metallic_rays.dir, metallic_loc);
    rays->orig = blend_vec128(&rays->orig, &metallic_rays.orig, metallic_loc);
  }
  if (!testz_128(lambertian_loc)) {
    RayCluster lambertian_rays = {
        .dir = rays->dir,
        .orig = hit_rec->orig,
    };
    scatter_lambertian(&lambertian_rays, hit_rec);

    rays->dir = blend_vec128(&rays->dir, &lambertian_rays.dir, lambertian_loc);
    rays->orig = blend_vec128(&rays->orig, &lambertian_rays.orig, lambertian_loc);
  }
  if (!testz_128(dielectric_loc)) {
    RayCluster dielectric_rays = {
        .dir = rays->dir,
        .orig = hit_rec->orig,
    };
    scatter_dielectric(&dielectric_rays, hit_rec);

    rays->dir = blend_vec128(&rays->dir, &dielectric_rays.dir, dielectric_loc);
    rays->orig = blend_vec128(&rays->orig, &dielectric_rays.orig, dielectric_loc);
  }
}

#else
#include <immintrin.h>

alignas(32) static const int metallic_types[8] = {
    MatType::metallic, MatType::metallic, MatType::metallic, MatType::metallic,
    MatType::metallic, MatType::metallic, MatType::metallic, MatType::metallic,
};

alignas(32) static const int lambertian_types[8] = {
    MatType::lambertian, MatType::lambertian, MatType::lambertian, MatType::lambertian,
    MatType::lambertian, MatType::lambertian, MatType::lambertian, MatType::lambertian,
};

alignas(32) static const int dielectric_types[8] = {
    MatType::dielectric, MatType::dielectric, MatType::dielectric, MatType::dielectric,
    MatType::dielectric, MatType::dielectric, MatType::dielectric, MatType::dielectric,
};

static LCGRand lcg_rand;
inline static void scatter_metallic(RayCluster* rays, const HitRecords* hit_rec) {
  Vec3_256 reflected = reflect(&rays->dir, &hit_rec->norm);
  normalize(&reflected);

  __m256 dp = dot(&reflected, &hit_rec->norm);
  __m256 greater_than_zero = _mm256_cmp_ps(dp, global::zeros, global::cmpnle);
  rays->dir = reflected & greater_than_zero;
};

[[nodiscard]] inline static __m256 near_zero(const Vec3_256* vec) {
  __m256 near_x = _mm256_cmp_ps(abs_256(vec->x), global::t_min_vec, global::cmplt);
  __m256 near_y = _mm256_cmp_ps(abs_256(vec->y), global::t_min_vec, global::cmplt);
  __m256 near_z = _mm256_cmp_ps(abs_256(vec->z), global::t_min_vec, global::cmplt);

  return _mm256_and_ps(near_x, _mm256_and_ps(near_y, near_z));
};

inline static void scatter_lambertian(RayCluster* rays, const HitRecords* hit_rec) {
  Vec3_256 rand_vec = lcg_rand.random_unit_vec();
  Vec3_256 scatter_dir = rand_vec + hit_rec->norm;

  //  rays->dir = blend_vec256(&scatter_dir, &hit_rec->norm, near_zero(&scatter_dir));
  rays->dir = scatter_dir;
}

[[nodiscard]] inline static __m256 reflectance(__m256 cos, __m256 ref_idx) {
  __m256 ref_low = global::white - ref_idx;
  __m256 ref_high = global::white + ref_idx;
  ref_high = _mm256_rcp_ps(ref_high);
  __m256 ref = ref_low * ref_high;
  ref *= ref;

  __m256 cos_sub = global::white - cos;
  // cos_sub^5
  __m256 cos_5 = cos_sub * cos_sub;
  cos_5 *= cos_sub;
  cos_5 *= cos_sub;
  cos_5 *= cos_sub;

  __m256 ref_sub = global::white - ref;
  return _mm256_fmadd_ps(ref_sub, cos_5, ref);
}

inline static void scatter_dielectric(RayCluster* rays, const HitRecords* hit_rec) {

  __m256 ri = _mm256_blendv_ps(global::ir_vec, global::rcp_ir_vec, hit_rec->front_face);
  Vec3_256 unit_dir = rays->dir;
  normalize(&unit_dir);

  Vec3_256 inverse_unit_dir = -unit_dir;

  __m256 cos_theta = dot(&inverse_unit_dir, &hit_rec->norm);
  cos_theta = _mm256_min_ps(cos_theta, global::white);

  __m256 sin_theta = _mm256_sqrt_ps(global::white - cos_theta * cos_theta);

  __m256 can_refract = ri * sin_theta;
  can_refract = _mm256_cmp_ps(can_refract, global::white, global::cmple);

  __m256 ref = reflectance(cos_theta, ri);
  __m256 rand_vec = lcg_rand.rand_in_range_256(0.f, 1.f);
  __m256 low_reflectance_loc = _mm256_cmp_ps(ref, rand_vec, global::cmple);
  __m256 refraction_loc = _mm256_and_ps(can_refract, low_reflectance_loc);
  __m256 reflection_loc = _mm256_xor_ps(refraction_loc, global::all_set);

  if (!_mm256_testz_ps(refraction_loc, refraction_loc)) {
    Vec3_256 refract_dir = refract(&unit_dir, &hit_rec->norm, ri);
    rays->dir = blend_vec256(&rays->dir, &refract_dir, refraction_loc);
  }
  if (!_mm256_testz_ps(reflection_loc, reflection_loc)) {
    Vec3_256 reflect_dir = reflect(&unit_dir, &hit_rec->norm);

    reflection_loc = _mm256_and_ps(reflection_loc, hit_rec->front_face);
    rays->dir = blend_vec256(&rays->dir, &reflect_dir, reflection_loc);
  }
}

inline static void scatter(RayCluster* rays, const HitRecords* hit_rec) {
  __m256i metallic_type = _mm256_load_si256((__m256i*)metallic_types);
  __m256i lambertian_type = _mm256_load_si256((__m256i*)lambertian_types);
  __m256i dielectric_type = _mm256_load_si256((__m256i*)dielectric_types);

  __m256i metallic_loc = _mm256_cmpeq_epi32(hit_rec->mat.type, metallic_type);
  __m256i lambertian_loc = _mm256_cmpeq_epi32(hit_rec->mat.type, lambertian_type);
  __m256i dielectric_loc = _mm256_cmpeq_epi32(hit_rec->mat.type, dielectric_type);

  if (!_mm256_testz_si256(metallic_loc, metallic_loc)) {
    RayCluster metallic_rays = {
        .dir = rays->dir,
        .orig = hit_rec->orig,
    };
    scatter_metallic(&metallic_rays, hit_rec);

    rays->dir = blend_vec256(&rays->dir, &metallic_rays.dir, (__m256)metallic_loc);
    rays->orig = blend_vec256(&rays->orig, &metallic_rays.orig, (__m256)metallic_loc);
  }
  if (!_mm256_testz_si256(lambertian_loc, lambertian_loc)) {
    RayCluster lambertian_rays = {
        .dir = rays->dir,
        .orig = hit_rec->orig,
    };
    scatter_lambertian(&lambertian_rays, hit_rec);

    rays->dir = blend_vec256(&rays->dir, &lambertian_rays.dir, (__m256)lambertian_loc);
    rays->orig = blend_vec256(&rays->orig, &lambertian_rays.orig, (__m256)lambertian_loc);
  }
  if (!_mm256_testz_si256(dielectric_loc, dielectric_loc)) {
    RayCluster dielectric_rays = {
        .dir = rays->dir,
        .orig = hit_rec->orig,
    };
    scatter_dielectric(&dielectric_rays, hit_rec);

    rays->dir = blend_vec256(&rays->dir, &dielectric_rays.dir, (__m256)dielectric_loc);
    rays->orig = blend_vec256(&rays->orig, &dielectric_rays.orig, (__m256)dielectric_loc);
  }
}

#endif