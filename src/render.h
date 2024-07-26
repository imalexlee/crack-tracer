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
#include <iostream>
#include <ostream>

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

void scatter_metallic(RayCluster* rays, const HitRecords* hit_rec);

void update_colors(Color_256* curr_colors, const Color_256* new_colors, __m256 update_mask);

[[nodiscard]] Color_256 ray_cluster_colors(RayCluster* rays, const Sphere* spheres, uint8_t depth);

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

void render(CharColor* data, uint32_t pixel_count, uint32_t offset);
