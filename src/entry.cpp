#include "constants.h"
#include "sphere.h"
#include "types.h"
#include <csignal>
#include <cstdio>
#include <immintrin.h>

constexpr Sphere spheres[SPHERE_NUM] = {{
    .x = 0.f,
    .y = 0.f,
    .z = 0.f,
    .r = 0.f,
}};

int main() {

  RayCluster rays = {
      .dir_x = _mm256_setzero_ps(),
      .dir_y = _mm256_setzero_ps(),
      .dir_z = {-1.f, -1.f, -1.f, -1.f, -1.f, -1.f, -1.f, -1.f},
  };

  // TODO: get rays

  find_sphere_hits(spheres, &rays, 100000.f);

  return 0;
}
