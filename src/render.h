#pragma once
#include "constants.h"
#include "globals.h"
#include "math.h"
#include "sphere.h"
#include "types.h"
#include "utils.h"
#include <cmath>
#include <csignal>
#include <cstdint>
#include <cstdio>
#include <cstring>
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

constexpr Material materials[SPHERE_NUM] = {
    {.atten = silver},
    //{.atten = silver},
};

constexpr Sphere spheres[SPHERE_NUM] = {
    {.center = {.x = 0.f, .y = -100.5f, .z = -1.f}, .mat = materials[0], .r = 100.f},
    {.center = {.x = 0.f, .y = 0.f, .z = -1.f}, .mat = materials[0], .r = 0.5f},
};

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
  __m256 no_hit_mask = zeros;
  HitRecords hit_rec;
  Color_256 colors{
      .r = global::white,
      .g = global::white,
      .b = global::white,
  };

  for (int i = 0; i < depth; i++) {

    hit_rec = find_sphere_hits(spheres, rays, INFINITY);

    __m256 new_hit_mask = _mm256_cmp_ps(hit_rec.t, zeros, CMPNLE);
    // or a mask when a value is not a hit, at any point. if all are zero,
    // break
    __m256 new_no_hit_mask = _mm256_xor_ps(new_hit_mask, global::all_set);
    no_hit_mask = _mm256_or_ps(no_hit_mask, new_no_hit_mask);
    if (_mm256_testz_ps(new_hit_mask, new_hit_mask)) {
      break;
    }

    scatter_metallic(rays, &hit_rec);
    update_colors(&colors, &hit_rec.mat.atten, new_hit_mask);
  }

  update_colors(&colors, &sky, no_hit_mask);

  return colors;
};

// gets us the first pixels row of sample directions.
// subsequent render iterations will simply scale this by row and column index
// to find where to take samples
consteval RayCluster generate_init_directions() {

  Vec3 top_left{
      .x = CAM_ORIGIN[0] - VIEWPORT_WIDTH / 2,
      .y = CAM_ORIGIN[1] + VIEWPORT_HEIGHT / 2,
      .z = CAM_ORIGIN[2] - FOCAL_LEN,
  };

  top_left.x += SAMPLE_DU;
  top_left.y += SAMPLE_DV;

  alignas(32) float x_arr[8];
  x_arr[0] = top_left.x;
  for (int i = 1; i < 8; i++) {
    x_arr[i] = x_arr[i - 1] + SAMPLE_DU;
  }

  RayCluster init_dirs = {
      .dir =
          {
              .x = {x_arr[0], x_arr[1], x_arr[2], x_arr[3], x_arr[4], x_arr[5], x_arr[6], x_arr[7]},
              .y = {top_left.y, top_left.y, top_left.y, top_left.y, top_left.y, top_left.y,
                    top_left.y, top_left.y},
              .z = {top_left.z, top_left.z, top_left.z, top_left.z, top_left.z, top_left.z,
                    top_left.z, top_left.z},
          },

  };

  return init_dirs;
}

inline static void render(CharColor* data) {
  static_assert((IMG_HEIGHT * IMG_WIDTH) % 8 == 0,
                "image size shall be divisible by 8 until I handle that case lol");
  constexpr RayCluster base_dirs = generate_init_directions();

  Color_256 sample_color;
  Color final_color;
  CharColor char_color;
  uint32_t write_pos;
  uint16_t row, col, sample_group;

  for (row = 0; row < IMG_HEIGHT; row++) {
    for (col = 0; col < IMG_WIDTH; col++) {

      sample_color.r = _mm256_setzero_ps();
      sample_color.g = _mm256_setzero_ps();
      sample_color.b = _mm256_setzero_ps();
      // 64 samples per pixel. 8 rays calculated 8 times
      //      if (row > IMG_HEIGHT / 2 && col > IMG_WIDTH / 2) {
      //        BREAKPOINT
      //      }
      for (sample_group = 0; sample_group < 8; sample_group++) {

        RayCluster samples = base_dirs;
        float x_scale = PIX_DU * col;
        float y_scale = (PIX_DV * row) + (sample_group * SAMPLE_DV);

        __m256 x_scale_vec = _mm256_broadcast_ss(&x_scale);
        __m256 y_scale_vec = _mm256_broadcast_ss(&y_scale);

        samples.dir.x = _mm256_add_ps(samples.dir.x, x_scale_vec);
        samples.dir.y = _mm256_add_ps(samples.dir.y, y_scale_vec);

        Color_256 new_colors = ray_cluster_colors(&samples, spheres, 10);
        sample_color.r = _mm256_add_ps(sample_color.r, new_colors.r);
        sample_color.g = _mm256_add_ps(sample_color.g, new_colors.g);
        sample_color.b = _mm256_add_ps(sample_color.b, new_colors.b);
      }
      // accumulate all color channels into first float of vec
      sample_color.r = _mm256_hadd_ps(sample_color.r, sample_color.r);
      sample_color.r = _mm256_hadd_ps(sample_color.r, sample_color.r);
      sample_color.r = _mm256_hadd_ps(sample_color.r, sample_color.r);

      sample_color.g = _mm256_hadd_ps(sample_color.g, sample_color.g);
      sample_color.g = _mm256_hadd_ps(sample_color.g, sample_color.g);
      sample_color.g = _mm256_hadd_ps(sample_color.g, sample_color.g);

      sample_color.b = _mm256_hadd_ps(sample_color.b, sample_color.b);
      sample_color.b = _mm256_hadd_ps(sample_color.b, sample_color.b);
      sample_color.b = _mm256_hadd_ps(sample_color.b, sample_color.b);

      _mm_store_ss(&final_color.r, _mm256_castps256_ps128(sample_color.r));
      _mm_store_ss(&final_color.g, _mm256_castps256_ps128(sample_color.g));
      _mm_store_ss(&final_color.b, _mm256_castps256_ps128(sample_color.b));

      // average by sample count. color / 64
      char_color.r = final_color.r * COLOR_MULTIPLIER;
      char_color.g = final_color.g * COLOR_MULTIPLIER;
      char_color.b = final_color.b * COLOR_MULTIPLIER;

      write_pos = col + row * IMG_WIDTH;
      data[write_pos] = char_color;
    }
  }
}
