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
const int sample_count = 10;

int main() {
  CharColor* img_data = (CharColor*)aligned_alloc(32, IMG_WIDTH * IMG_HEIGHT * sizeof(CharColor));

  if constexpr (MULTITHREADING_ENABLED) {

    auto start_time = system_clock::now();
    std::array<std::thread, THREAD_COUNT> threads;
    for (size_t idx = 0; idx < threads.size(); idx++) {

      constexpr uint32_t chunk_size = IMG_WIDTH * (IMG_HEIGHT / threads.size());
      threads[idx] = std::thread(render, img_data, chunk_size, idx * chunk_size);
    }

    for (size_t idx = 0; idx < threads.size(); idx++) {
      threads[idx].join();
    }
    auto end_time = system_clock::now();
    auto dur = duration<float>(end_time - start_time);
    float milli = duration_cast<microseconds>(dur).count() / 1000.f;
    printf("render time (ms): %f\n", milli);
  } else {
    double total_milli = 0.0;
    for (int i = 0; i < sample_count; i++) {
      auto start_time = system_clock::now();
      render(img_data, IMG_WIDTH * IMG_HEIGHT, 0);
      auto end_time = system_clock::now();
      auto dur = duration<float>(end_time - start_time);
      float milli = duration_cast<microseconds>(dur).count() / 1000.f;
      total_milli += milli;
      printf("sample %d render time (ms): %f\n", i, milli);
    }
    printf("total render time (ms): %f\n", total_milli / sample_count);
  }

  stbi_write_png("out.png", IMG_WIDTH, IMG_HEIGHT, 3, img_data, IMG_WIDTH * sizeof(CharColor));
}
