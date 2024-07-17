#include "../src/sphere.h"
#include "../src/utils.h"
#include <cmath>
#include <cstdio>
#include <immintrin.h>

const RayCluster rays{
    .dir_x = _mm256_set_ps(-.6f, -.2f, .2f, .6f, -.6f, -.2f, .2f, .6f),
    .dir_y = _mm256_set_ps(.4f, .4f, .4f, .4f, -.4f, -.4f, -.4f, -.4f),
    .dir_z = _mm256_set_ps(-1.f, -1.f, -1.f, -1.f, -1.f, -1.f, -1.f, -1.f),
};

const Sphere sphere{
    .x = 0.f,
    .y = 0.f,
    .z = -1.f,
    .r = .5f,
};

void test_sphere_hit() {
  printf("TESTING SPHERE_HIT\n");
  __m256 t_vals = sphere_hit(&rays, 1000.f, sphere);
  print_vec_256(t_vals);
  printf("\n");
};

void test_update_sphere_cluster() {
  printf("TESTING UPDATE_SPHERE_CLUSTER\n");
  SphereCluster sphere_cluster{
      .x = _mm256_setzero_ps(),
      .y = _mm256_setzero_ps(),
      .z = _mm256_setzero_ps(),
      .r = _mm256_setzero_ps(),
  };
  const Sphere rando_sphere1{
      .x = 69.f,
      .y = 420.f,
      .z = 21.f,
      .r = .8f,
  };

  const Sphere rando_sphere2{
      .x = 100.f,
      .y = -2.3f,
      .z = 800.f,
      .r = 0.39f,
  };

  uint32_t a = 0xFFFFFFFF;
  float set = *((float*)&a);

  __m256 mask = _mm256_set_ps(set, 0.f, set, 0.f, set, 0.f, set, 0.f);
  update_sphere_cluster(&sphere_cluster, rando_sphere1, mask);

  printf("iter 1\n");
  print_vec_256(sphere_cluster.x);
  print_vec_256(sphere_cluster.y);
  print_vec_256(sphere_cluster.z);
  print_vec_256(sphere_cluster.r);

  mask = _mm256_set_ps(0.f, 0.f, 0.f, set, set, set, set, 0.f);
  update_sphere_cluster(&sphere_cluster, rando_sphere2, mask);

  printf("iter 2\n");
  print_vec_256(sphere_cluster.x);
  print_vec_256(sphere_cluster.y);
  print_vec_256(sphere_cluster.z);
  print_vec_256(sphere_cluster.r);

  printf("\n");
}

int main() {
  test_sphere_hit();
  test_update_sphere_cluster();
  return 0;
}
