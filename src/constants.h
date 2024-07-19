#pragma once
#include <cfloat>
#include <cstdint>

constexpr uint8_t SPHERE_NUM = 1;
constexpr uint8_t CMPEQ = 0;
constexpr uint8_t CMPLT = 1;
constexpr uint8_t CMPNEQ = 4;
constexpr uint8_t CMPNLT = 5;
constexpr uint8_t SHUF_ALL_FIRST = 0;
constexpr uint8_t SHUF_ALL_SECOND = 85;
constexpr uint8_t SHUF_ALL_THIRD = 170;
constexpr uint8_t RAY_DEPTH = 20;
constexpr float FLOAT_MAX = FLT_MAX;
alignas(32) constexpr float CAM_ORIGIN[4] = {0.f, 0.f, 0.f, 0.f};
