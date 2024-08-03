#pragma once
#include "immintrin.h"
#include <cfloat>
#include <cstdint>

enum RenderMode {
  png,
  real_time,
};

namespace global {
  constexpr uint8_t sphere_num = 3;
  constexpr uint8_t sample_group_num = 4;
  constexpr uint8_t cmpeq = 0;
  constexpr uint8_t cmplt = 1;
  constexpr uint8_t cmpneq = 4;
  constexpr uint8_t cmpnlt = 5;
  constexpr uint8_t cmpnle = 6;
  constexpr uint8_t shuf_all_first = 0;
  constexpr uint8_t shuf_all_second = 85;
  constexpr uint8_t shuf_all_third = 170;
  constexpr uint8_t ray_depth = 20;
  constexpr float float_max = FLT_MAX;
  constexpr uint16_t img_width = 1920;
  constexpr uint16_t img_height = 1080;
  constexpr float viewport_height = 2.f;
  constexpr float viewport_width = viewport_height * (float(img_width) / img_height);
  constexpr float pix_du = viewport_width / img_width;
  constexpr float pix_dv = -viewport_height / img_height;
  // 8 evenly spread out ray directions. (space-around)
  constexpr float sample_du = pix_du / 9;
  constexpr float sample_dv = pix_dv / (sample_group_num + 1);
  constexpr float focal_len = 1.0;
  constexpr float color_multiplier = 255.f / (sample_group_num * 8);
  constexpr uint8_t thread_count = 50;
  constexpr RenderMode active_render_mode = RenderMode::png;
  alignas(32) constexpr float cam_origin[4] = {0.f, 0.f, 0.0f, 0.f};
  const __m256 all_set = _mm256_cmp_ps(_mm256_setzero_ps(), _mm256_setzero_ps(), cmpeq);
  constexpr __m256 white = {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f};
  constexpr __m256 t_min_vec = {
      0.001f, 0.001f, 0.001f, 0.001f, 0.001f, 0.001f, 0.001f, 0.001f,
  };
} // namespace global
