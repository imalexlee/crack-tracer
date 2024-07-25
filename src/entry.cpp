#include "constants.h"
#include "render.h"
#include "types.h"
#include <array>
#include <chrono>
#include <cmath>
#include <csignal>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <thread>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <immintrin.h>
#include <stb_image_write.h>

using namespace std::chrono;

int main() {
  CharColor* img_data = (CharColor*)malloc(IMG_WIDTH * IMG_HEIGHT * sizeof(CharColor));

  auto start_time = system_clock::now();

  if constexpr (MULTITHREADING_ENABLED) {

    std::array<std::thread, THREAD_COUNT> threads;
    for (size_t idx = 0; idx < threads.size(); idx++) {

      constexpr uint32_t chunk_size = IMG_WIDTH * (IMG_HEIGHT / threads.size());
      threads[idx] = std::thread(render, img_data, chunk_size, idx * chunk_size);
    }

    for (size_t idx = 0; idx < threads.size(); idx++) {
      threads[idx].join();
    }
  } else {
    render(img_data, IMG_WIDTH * IMG_HEIGHT, 0);
  }

  auto end_time = system_clock::now();
  auto dur = duration<float>(end_time - start_time);
  float milli = duration_cast<microseconds>(dur).count() / 1000.f;
  printf("render time (ms): %f\n", milli);

  stbi_write_png("out.png", IMG_WIDTH, IMG_HEIGHT, 3, img_data, IMG_WIDTH * sizeof(CharColor));
}
