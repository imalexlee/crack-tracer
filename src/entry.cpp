#include "constants.h"
#include "render.h"
#include "types.h"
#include <chrono>
#include <cmath>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <immintrin.h>
#include <stb_image_write.h>

using namespace std::chrono;

int main() {
  static_assert((IMG_HEIGHT * IMG_WIDTH) % 8 == 0,
                "image size shall be divisible by 8 until I handle that case lol");

  CharColor* img_data = (CharColor*)malloc(IMG_WIDTH * IMG_HEIGHT * sizeof(CharColor));

  std::vector<float> avg_times;
  avg_times.reserve(128);
  int sample_count = 20;
  for (int i = 12; i < 150; i++) {
    float avg_time = 0.f;
    for (int sample_num = 0; sample_num < sample_count; sample_num++) {
      auto start_time = system_clock::now();
      std::vector<std::thread> threads(i);
      for (size_t idx = 0; idx < threads.size(); idx++) {
        uint32_t chunk_size = IMG_WIDTH * (IMG_HEIGHT / threads.size());
        threads[idx] = std::thread(render, img_data, chunk_size, idx * chunk_size);
      }

      for (size_t idx = 0; idx < threads.size(); idx++) {
        threads[idx].join();
      }
      auto end_time = system_clock::now();
      auto dur = duration<float>(end_time - start_time);
      avg_time += duration_cast<microseconds>(dur).count() / 1000.f;
    }
    avg_time /= sample_count;
    std::cout << i << " " << avg_time << '\n';
    avg_times.push_back(avg_time);
  }

  float lowest_time = 10000.f;
  int lowest_idx = 10000;
  for (size_t i = 0; i < avg_times.size(); i++) {
    if (avg_times[i] < lowest_time) {
      lowest_time = avg_times[i];
      lowest_idx = i + 12;
    }
  }

  printf("lowest time: %f\n", lowest_time);
  printf("lowest thread count: %d", lowest_idx);

  stbi_write_png("out.png", IMG_WIDTH, IMG_HEIGHT, 3, img_data, IMG_WIDTH * sizeof(CharColor));
}
