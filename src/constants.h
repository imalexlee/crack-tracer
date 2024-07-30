#pragma once
#include <cfloat>
#include <cstdint>

enum RENDER_MODE {
  PNG,
  REAL_TIME,
};

constexpr uint8_t SPHERE_NUM = 3;
constexpr uint8_t SAMPLE_GROUP_NUM = 4;
constexpr uint8_t CMPEQ = 0;
constexpr uint8_t CMPLT = 1;
constexpr uint8_t CMPNEQ = 4;
constexpr uint8_t CMPNLT = 5;
constexpr uint8_t CMPNLE = 6;
constexpr uint8_t SHUF_ALL_FIRST = 0;
constexpr uint8_t SHUF_ALL_SECOND = 85;
constexpr uint8_t SHUF_ALL_THIRD = 170;
constexpr uint8_t RAY_DEPTH = 20;
constexpr float FLOAT_MAX = FLT_MAX;
constexpr uint16_t IMG_WIDTH = 1920;
constexpr uint16_t IMG_HEIGHT = 1080;
constexpr float VIEWPORT_HEIGHT = 2.f;
constexpr float VIEWPORT_WIDTH = VIEWPORT_HEIGHT * (float(IMG_WIDTH) / IMG_HEIGHT);
constexpr float PIX_DU = VIEWPORT_WIDTH / IMG_WIDTH;
constexpr float PIX_DV = -VIEWPORT_HEIGHT / IMG_HEIGHT;
// 8 evenly spread out ray directions. (space-around)
constexpr float SAMPLE_DU = PIX_DU / 9;
constexpr float SAMPLE_DV = PIX_DV / (SAMPLE_GROUP_NUM + 1);
constexpr float FOCAL_LEN = 1.0;
constexpr float COLOR_MULTIPLIER = 255.f / (SAMPLE_GROUP_NUM * 8);
constexpr uint8_t THREAD_COUNT = 50;
constexpr RENDER_MODE ACTIVE_RENDER_MODE = RENDER_MODE::REAL_TIME;
alignas(32) constexpr float CAM_ORIGIN[4] = {0.f, 0.f, 0.0f, 0.f};
