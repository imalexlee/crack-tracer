#pragma once
#include "constants.h"
#include "globals.h"
#include "math.h"
#include "sphere.h"
#include "types.h"
#include <cmath>
#include <csignal>
#include <cstdint>
#include <cstdio>
#include <immintrin.h>

constexpr Color_256 silver_attenuation = {
    .r = {0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f},
    .g = {0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f},
    .b = {0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f, 0.66f},
};

constexpr Color silver = {
    .r = 0.66f,
    .g = 0.66f,
    .b = 0.66f,
};

constexpr Material materials[SPHERE_NUM] = {{
    .atten = silver,
}};

constexpr Sphere spheres[SPHERE_NUM] = {{
    .center = {.x = 0.f, .y = 0.f, .z = -1.f},
    .mat = materials[0],
    .r = 0.5f,
}};

constexpr Color_256 sky = {
    .r = {0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
    .g = {0.7f, 0.7f, 0.7f, 0.7f, 0.7f, 0.7f, 0.7f, 0.7f},
    .b = {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f},
};

inline static void scatter_metallic(RayCluster* rays, const HitRecords* hit_rec) {
  // reflect ray direction about the hit record normal
  reflect(&rays->dir, &hit_rec->norm);
  normalize(&rays->dir);
};

inline static void update_colors(Color_256* curr_colors, const Color_256* new_colors,
                                 __m256 update_mask) {

  // multiply current colors by the attenuation of new hits.
  // fill 1.0 for no hits in order to preserve current colors when multiplying
  __m256 r = _mm256_and_ps(new_colors->r, update_mask);
  __m256 g = _mm256_and_ps(new_colors->g, update_mask);
  __m256 b = _mm256_and_ps(new_colors->b, update_mask);

  __m256 new_no_hit_mask = _mm256_xor_ps(update_mask, global::all_set);
  __m256 preserve_curr = _mm256_and_ps(global::white, new_no_hit_mask);

  // 1.0 where no new hit occured and we want to preserve the prev color.
  // new r,g,b values where a new hit occured and we want to scale prev color by
  r = _mm256_add_ps(r, preserve_curr);
  g = _mm256_add_ps(g, preserve_curr);
  b = _mm256_add_ps(b, preserve_curr);

  curr_colors->r = _mm256_mul_ps(curr_colors->r, r);
  curr_colors->g = _mm256_mul_ps(curr_colors->g, g);
  curr_colors->b = _mm256_mul_ps(curr_colors->b, b);
}

[[nodiscard]] inline static Color_256 ray_cluster_colors(RayCluster* rays, const Sphere* spheres,
                                                         uint8_t depth) {
  const __m256 zeros = _mm256_setzero_ps();
  // will be used to add a sky tint to rays that at some point bounce off into space.
  // if a ray never bounces away (within amount of bounces set by depth), the
  // hit_mask will be all set (packed floats) and the sky tint will not affect its final color
  __m256 hit_mask = zeros;
  HitRecords hit_rec;
  Color_256 colors{
      .r = global::white,
      .g = global::white,
      .b = global::white,
  };

  for (int i = 0; i < depth; i++) {

    hit_rec = find_sphere_hits(spheres, rays, INFINITY);

    __m256 new_hit_mask = _mm256_cmp_ps(hit_rec.t, zeros, CMPEQ);
    if (_mm256_testz_ps(new_hit_mask, new_hit_mask)) {
      break;
    }

    scatter_metallic(rays, &hit_rec);
    update_colors(&colors, &hit_rec.mat.atten, new_hit_mask);

    // update where any rays bounced off into space (no hits)
    hit_mask = _mm256_or_ps(hit_mask, new_hit_mask);
  }

  // negate hits since we want to apply sky color to where rays
  // bounced away at some point in the loop
  __m256 no_hit_mask = _mm256_xor_ps(hit_mask, global::all_set);
  update_colors(&colors, &sky, no_hit_mask);

  return colors;
};
