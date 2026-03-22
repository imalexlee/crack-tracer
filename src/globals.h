#pragma once
#include "types.h"
#include <cfloat>
#include <cstdint>

enum RenderMode {
  png,
  real_time,
};

#ifdef __ARM_NEON
#include <arm_neon.h>

constexpr Color_128 night = {
    .x = {0.02, 0.02, 0.02, 0.02},
    .y = {0.08, 0.08, 0.08, 0.08},
    .z = {0.35, 0.35, 0.35, 0.35},
};

namespace global {
  // each group calculates 8 samples.
  constexpr uint16_t sample_group_num = 500;
  constexpr uint8_t shuf_all_first = 0;
  constexpr uint8_t shuf_all_second = 85;
  constexpr uint8_t shuf_all_third = 170;
  constexpr uint8_t ray_depth = 20;
  constexpr float float_max = FLT_MAX;
  constexpr uint16_t img_width = 2560;
  constexpr uint16_t img_height = 1440;
  // constexpr uint16_t img_width = 1920;
  // constexpr uint16_t img_height = 1080;
  constexpr float viewport_height = 2.f;
  constexpr float viewport_width = viewport_height * (float(img_width) / img_height);
  constexpr float pix_du = viewport_width / img_width;
  constexpr float pix_dv = -viewport_height / img_height;
  // 8 evenly spread out ray directions. (space-around)
  constexpr float sample_du = pix_du / 9;
  constexpr float sample_dv = pix_dv / (sample_group_num + 1);
  constexpr float focal_len = 1.0;
  constexpr float color_multiplier = 255.f / (sample_group_num * 8);
  constexpr uint8_t thread_count = 4;
  constexpr RenderMode active_render_mode = RenderMode::png;
  // index of refraction
  constexpr float ir = 1.5;
  // constexpr float ir = 1.5;
  constexpr float32x4_t ir_vec = {ir, ir, ir, ir};
  constexpr float rcp_ir = 1.f / ir;
  constexpr float32x4_t rcp_ir_vec = {rcp_ir, rcp_ir, rcp_ir, rcp_ir};
  alignas(32) constexpr float cam_origin[4] = {0.f, 0.f, 0.0f, 0.f};
  const uint32x4_t sign_bit = vdupq_n_u32((uint32_t)1u << 31);
  const uint32x4_t all_set = vceqq_f32(vdupq_n_f32(0.0f), vdupq_n_f32(0.0f));
  constexpr float32x4_t zeros = {0, 0, 0, 0};
  constexpr float32x4_t white = {1.f, 1.f, 1.f, 1.f};
  constexpr float t_min = 0.0013f;
  const float32x4_t t_min_vec = {t_min, t_min, t_min, t_min};
  const Color_128 background_color = {.x = white, .y = white, .z = white};
} // namespace global

#else
#include <immintrin.h>

constexpr Color_256 night = {
    .x = {0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02},
    .y = {0.08, 0.08, 0.08, 0.08, 0.08, 0.08, 0.08, 0.08},
    .z = {0.35, 0.35, 0.35, 0.35, 0.35, 0.35, 0.35, 0.35},
};

namespace global {
  // each group calculates 8 samples.
  constexpr uint16_t sample_group_num = 500;
  constexpr uint8_t cmpeq = 0;
  constexpr uint8_t cmplt = 1;
  constexpr uint8_t cmple = 2;
  constexpr uint8_t cmpneq = 4;
  constexpr uint8_t cmpnlt = 5;
  constexpr uint8_t cmpnle = 6;
  constexpr uint8_t shuf_all_first = 0;
  constexpr uint8_t shuf_all_second = 85;
  constexpr uint8_t shuf_all_third = 170;
  constexpr uint8_t ray_depth = 20;
  constexpr float float_max = FLT_MAX;
  constexpr uint16_t img_width = 2560;
  constexpr uint16_t img_height = 1440;
  // constexpr uint16_t img_width = 1920;
  // constexpr uint16_t img_height = 1080;
  constexpr float viewport_height = 2.f;
  constexpr float viewport_width = viewport_height * (float(img_width) / img_height);
  constexpr float pix_du = viewport_width / img_width;
  constexpr float pix_dv = -viewport_height / img_height;
  // 8 evenly spread out ray directions. (space-around)
  constexpr float sample_du = pix_du / 9;
  constexpr float sample_dv = pix_dv / (sample_group_num + 1);
  constexpr float focal_len = 1.0;
  constexpr float color_multiplier = 255.f / (sample_group_num * 8);
  constexpr uint8_t thread_count = 60;
  constexpr RenderMode active_render_mode = RenderMode::png;
  // index of refraction
  constexpr float ir = 1.5;
  // constexpr float ir = 1.5;
  constexpr __m256 ir_vec = {ir, ir, ir, ir, ir, ir, ir, ir};
  constexpr float rcp_ir = 1.f / ir;
  constexpr __m256 rcp_ir_vec = {rcp_ir, rcp_ir, rcp_ir, rcp_ir, rcp_ir, rcp_ir, rcp_ir, rcp_ir};
  alignas(32) constexpr float cam_origin[4] = {0.f, 0.f, 0.0f, 0.f};
  const __m256 all_set = _mm256_cmp_ps(_mm256_setzero_ps(), _mm256_setzero_ps(), cmpeq);
  constexpr __m256 zeros = {0, 0, 0, 0, 0, 0, 0, 0};
  constexpr __m256 white = {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f};
  constexpr float t_min = 0.0013f;
  constexpr __m256 t_min_vec = {t_min, t_min, t_min, t_min, t_min, t_min, t_min, t_min};
  constexpr Color_256 background_color = {.x = white, .y = white, .z = white};
} // namespace global

#endif