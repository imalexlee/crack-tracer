#include "constants.h"
#include "render.h"
#include "types.h"
#include <chrono>
#include <cmath>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <immintrin.h>
#include <stb_image_write.h>

using namespace std::chrono;
int main() {
  CharColor* img_data = (CharColor*)malloc(IMG_WIDTH * IMG_HEIGHT * sizeof(CharColor));

  auto start_time = system_clock::now();

  render(img_data);

  auto end_time = system_clock::now();
  auto dur = duration<float>(end_time - start_time);
  float milli = duration_cast<microseconds>(dur).count() / 1000.f;
  printf("render time (ms): %f\n", milli);

  stbi_write_png("out.png", IMG_WIDTH, IMG_HEIGHT, 3, img_data, IMG_WIDTH * sizeof(CharColor));
}
