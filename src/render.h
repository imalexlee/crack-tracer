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

constexpr Color red = {
    .r = 0.90f,
    .g = 0.20f,
    .b = 0.20f,
};
constexpr Color gold = {
    .r = 0.85f,
    .g = 0.70f,
    .b = 0.24f,
};

constexpr Material materials[SPHERE_NUM] = {
    {.atten = silver},
    {.atten = red},
    {.atten = gold},
};

constexpr Sphere spheres[SPHERE_NUM] = {
    {.center = {.x = 0.f, .y = 0.f, .z = -1.2f}, .mat = materials[0], .r = 0.5f},
    {.center = {.x = -1.f, .y = 0.f, .z = -1.f}, .mat = materials[1], .r = 0.4f},
    {.center = {.x = 1.f, .y = 0.f, .z = -1.f}, .mat = materials[2], .r = 0.4f},
    //{.center = {.x = 0.f, .y = -100.5f, .z = -1.f}, .mat = materials[2], .r = 100.f},
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

inline static Color_256 ray_cluster_colors(RayCluster* rays, const Sphere* spheres, uint8_t depth) {
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

    find_sphere_hits(&hit_rec, spheres, rays, INFINITY);

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

inline static void write_out_color_buf(const Color* color_buf, CharColor* img_buf,
                                       uint32_t write_pos) {
  // when temp buffer is full, flush to the image
  __m256 cm = _mm256_broadcast_ss(&COLOR_MULTIPLIER);
  __m256 colors_1_f32 = _mm256_mul_ps(_mm256_load_ps((float*)color_buf), cm);
  __m256 colors_2_f32 = _mm256_mul_ps(_mm256_load_ps((float*)(color_buf) + 8), cm);
  __m256 colors_3_f32 = _mm256_mul_ps(_mm256_load_ps((float*)(color_buf) + 16), cm);
  __m256 colors_4_f32 = _mm256_mul_ps(_mm256_load_ps((float*)(color_buf) + 24), cm);
  __m256 colors_5_f32 = _mm256_mul_ps(_mm256_load_ps((float*)(color_buf) + 32), cm);
  __m256 colors_6_f32 = _mm256_mul_ps(_mm256_load_ps((float*)(color_buf) + 40), cm);
  __m256 colors_7_f32 = _mm256_mul_ps(_mm256_load_ps((float*)(color_buf) + 48), cm);
  __m256 colors_8_f32 = _mm256_mul_ps(_mm256_load_ps((float*)(color_buf) + 56), cm);
  __m256 colors_9_f32 = _mm256_mul_ps(_mm256_load_ps((float*)(color_buf) + 64), cm);
  __m256 colors_10_f32 = _mm256_mul_ps(_mm256_load_ps((float*)(color_buf) + 72), cm);
  __m256 colors_11_f32 = _mm256_mul_ps(_mm256_load_ps((float*)(color_buf) + 80), cm);
  __m256 colors_12_f32 = _mm256_mul_ps(_mm256_load_ps((float*)(color_buf) + 88), cm);

  __m256i colors_1_i32 = _mm256_cvtps_epi32(colors_1_f32);
  __m256i colors_2_i32 = _mm256_cvtps_epi32(colors_2_f32);
  __m256i colors_3_i32 = _mm256_cvtps_epi32(colors_3_f32);
  __m256i colors_4_i32 = _mm256_cvtps_epi32(colors_4_f32);
  __m256i colors_5_i32 = _mm256_cvtps_epi32(colors_5_f32);
  __m256i colors_6_i32 = _mm256_cvtps_epi32(colors_6_f32);
  __m256i colors_7_i32 = _mm256_cvtps_epi32(colors_7_f32);
  __m256i colors_8_i32 = _mm256_cvtps_epi32(colors_8_f32);
  __m256i colors_9_i32 = _mm256_cvtps_epi32(colors_9_f32);
  __m256i colors_10_i32 = _mm256_cvtps_epi32(colors_10_f32);
  __m256i colors_11_i32 = _mm256_cvtps_epi32(colors_11_f32);
  __m256i colors_12_i32 = _mm256_cvtps_epi32(colors_12_f32);

  const uint8_t BOTH_LOW_XMMWORD = 32;
  const uint8_t BOTH_HIGH_XMMWORD = 49;
  __m256i temp_permute_1 = _mm256_permute2x128_si256(colors_1_i32, colors_2_i32, BOTH_LOW_XMMWORD);
  __m256i temp_permute_2 = _mm256_permute2x128_si256(colors_1_i32, colors_2_i32, BOTH_HIGH_XMMWORD);
  __m256i temp_permute_3 = _mm256_permute2x128_si256(colors_3_i32, colors_4_i32, BOTH_LOW_XMMWORD);
  __m256i temp_permute_4 = _mm256_permute2x128_si256(colors_3_i32, colors_4_i32, BOTH_HIGH_XMMWORD);
  __m256i temp_permute_5 = _mm256_permute2x128_si256(colors_5_i32, colors_6_i32, BOTH_LOW_XMMWORD);
  __m256i temp_permute_6 = _mm256_permute2x128_si256(colors_5_i32, colors_6_i32, BOTH_HIGH_XMMWORD);
  __m256i temp_permute_7 = _mm256_permute2x128_si256(colors_7_i32, colors_8_i32, BOTH_LOW_XMMWORD);
  __m256i temp_permute_8 = _mm256_permute2x128_si256(colors_7_i32, colors_8_i32, BOTH_HIGH_XMMWORD);
  __m256i temp_permute_9 = _mm256_permute2x128_si256(colors_9_i32, colors_10_i32, BOTH_LOW_XMMWORD);
  __m256i temp_permute_10 =
      _mm256_permute2x128_si256(colors_9_i32, colors_10_i32, BOTH_HIGH_XMMWORD);
  __m256i temp_permute_11 =
      _mm256_permute2x128_si256(colors_11_i32, colors_12_i32, BOTH_LOW_XMMWORD);
  __m256i temp_permute_12 =
      _mm256_permute2x128_si256(colors_11_i32, colors_12_i32, BOTH_HIGH_XMMWORD);

  __m256i colors_1_i16 = _mm256_packs_epi32(temp_permute_1, temp_permute_2);
  __m256i colors_2_i16 = _mm256_packs_epi32(temp_permute_3, temp_permute_4);
  __m256i colors_3_i16 = _mm256_packs_epi32(temp_permute_5, temp_permute_6);
  __m256i colors_4_i16 = _mm256_packs_epi32(temp_permute_7, temp_permute_8);
  __m256i colors_5_i16 = _mm256_packs_epi32(temp_permute_9, temp_permute_10);
  __m256i colors_6_i16 = _mm256_packs_epi32(temp_permute_11, temp_permute_12);

  temp_permute_1 = _mm256_permute2x128_si256(colors_1_i16, colors_2_i16, BOTH_LOW_XMMWORD);
  temp_permute_2 = _mm256_permute2x128_si256(colors_1_i16, colors_2_i16, BOTH_HIGH_XMMWORD);
  temp_permute_3 = _mm256_permute2x128_si256(colors_3_i16, colors_4_i16, BOTH_LOW_XMMWORD);
  temp_permute_4 = _mm256_permute2x128_si256(colors_3_i16, colors_4_i16, BOTH_HIGH_XMMWORD);
  temp_permute_5 = _mm256_permute2x128_si256(colors_5_i16, colors_6_i16, BOTH_LOW_XMMWORD);
  temp_permute_6 = _mm256_permute2x128_si256(colors_5_i16, colors_6_i16, BOTH_HIGH_XMMWORD);

  __m256i colors_1_u8 = _mm256_packus_epi16(temp_permute_1, temp_permute_2);
  __m256i colors_2_u8 = _mm256_packus_epi16(temp_permute_3, temp_permute_4);
  __m256i colors_3_u8 = _mm256_packus_epi16(temp_permute_5, temp_permute_6);

  _mm256_stream_si256(((__m256i*)img_buf) + write_pos, colors_1_u8);
  _mm256_stream_si256(((__m256i*)img_buf) + write_pos + 1, colors_2_u8);
  _mm256_stream_si256(((__m256i*)img_buf) + write_pos + 2, colors_3_u8);
}

inline static void render(CharColor* img_buf, uint32_t pixel_count, uint32_t pix_offset) {
  constexpr RayCluster base_dirs = generate_init_directions();

  Color_256 sample_color;

  alignas(32) Color color_buf[32];
  uint8_t color_buf_idx = 0;
  uint32_t write_pos = (pix_offset / 32) * 3;
  uint16_t sample_group;
  uint32_t row = pix_offset / IMG_WIDTH;
  uint32_t col = pix_offset % IMG_WIDTH;
  uint32_t end_row = (pix_offset + pixel_count - 1) / IMG_WIDTH;
  uint32_t end_col = (pix_offset + pixel_count - 1) % IMG_WIDTH;

  for (; row <= end_row; row++) {
    while (col <= end_col) {
      sample_color.r = _mm256_setzero_ps();
      sample_color.g = _mm256_setzero_ps();
      sample_color.b = _mm256_setzero_ps();
      // 64 samples per pixel. 8 rays calculated 8 times
      // if (row > IMG_HEIGHT / 2 && col > IMG_WIDTH / 2) {
      //  BREAKPOINT
      //}
      for (sample_group = 0; sample_group < SAMPLE_GROUP_NUM; sample_group++) {

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

      _mm_store_ss(&color_buf[color_buf_idx].r, _mm256_castps256_ps128(sample_color.r));
      _mm_store_ss(&color_buf[color_buf_idx].g, _mm256_castps256_ps128(sample_color.g));
      _mm_store_ss(&color_buf[color_buf_idx].b, _mm256_castps256_ps128(sample_color.b));

      color_buf_idx++;
      col++;

      if (color_buf_idx != 32) {
        continue;
      }

      write_out_color_buf(color_buf, img_buf, write_pos);
      write_pos += 3;

      color_buf_idx = 0;
    }
    col = 0;
  }
}
