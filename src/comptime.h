#pragma once
#include "globals.h"
#include "types.h"

namespace comptime {
  // gets us the first pixels row of sample directions during compile time.
  //
  // subsequent render iterations will simply scale this by
  // row and column index to find where to take samples
  consteval Vec3_256 init_ray_directions() {

    Vec3 top_left{
        .x = global::cam_origin[0] - global::viewport_width / 2,
        .y = global::cam_origin[1] + global::viewport_height / 2,
        .z = global::cam_origin[2] - global::focal_len,
    };

    top_left.x += global::sample_du;
    top_left.y += global::sample_dv;

    alignas(32) float x_arr[8];
    x_arr[0] = top_left.x;
    for (int i = 1; i < 8; i++) {
      x_arr[i] = x_arr[i - 1] + global::sample_du;
    }

    Vec3_256 init_dirs = {
        .x = {x_arr[0], x_arr[1], x_arr[2], x_arr[3], x_arr[4], x_arr[5], x_arr[6], x_arr[7]},
        .y = {top_left.y, top_left.y, top_left.y, top_left.y, top_left.y, top_left.y, top_left.y,
              top_left.y},
        .z = {top_left.z, top_left.z, top_left.z, top_left.z, top_left.z, top_left.z, top_left.z,
              top_left.z},

    };

    return init_dirs;
  }
  consteval __m256i init_rseed_arr() {
    int rseed_arr[8];
    rseed_arr[0] = 0;
    for (size_t i = 1; i < 8; i++) {
      rseed_arr[i] = (rseed_arr[i - 1] * 11035152453u + 12345u) & RAND_MAX;
    }
    __m256 rseed_vec = {
        (float)rseed_arr[0], (float)rseed_arr[1], (float)rseed_arr[2], (float)rseed_arr[3],
        (float)rseed_arr[4], (float)rseed_arr[5], (float)rseed_arr[6], (float)rseed_arr[7],
    };

    return (__m256i)rseed_vec;
  }

}; // namespace comptime
