#pragma once
#include <cstdint>

constexpr uint8_t SPHERE_NUM = 1;
constexpr uint8_t CMPEQPS = 0;
constexpr uint8_t CMPLTPS = 1;
constexpr uint8_t CMPNLTPS = 5;
constexpr uint8_t SHUF_ALL_FIRST = 0;
constexpr uint8_t SHUF_ALL_SECOND = 85;
constexpr uint8_t SHUF_ALL_THIRD = 170;
constexpr float FLOAT_INF = 0x7f800000;
alignas(32) constexpr float CAM_ORIGIN[4] = {0.f, 0.f, 0.f, 0.f};
