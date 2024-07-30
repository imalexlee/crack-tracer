#include "constants.h"
#include "render.h"
#include "types.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL_keycode.h>
#include <cmath>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <thread>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <SDL2/SDL.h>
#include <SDL2/SDL_test_images.h>
#include <immintrin.h>
#include <stb_image_write.h>

using namespace std::chrono;

int main() {
  SDL_Window* win = NULL;
  SDL_Renderer* renderer = NULL;
  CharColor* img_data = (CharColor*)aligned_alloc(32, IMG_WIDTH * IMG_HEIGHT * sizeof(CharColor));

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    return 1;

  win = SDL_CreateWindow("Image Loading", 100, 100, IMG_WIDTH, IMG_HEIGHT, 0);
  renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

  SDL_Texture* buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24,
                                          SDL_TEXTUREACCESS_STREAMING, IMG_WIDTH, IMG_HEIGHT);

  int pitch = IMG_WIDTH * sizeof(CharColor);

  while (1) {

    // event handling
    SDL_Event e;
    if (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        break;
      else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
        break;
    }

    SDL_RenderClear(renderer);

    SDL_LockTexture(buffer, NULL, (void**)(&img_data), &pitch);

    render(img_data, IMG_WIDTH * IMG_HEIGHT, 0);

    SDL_UnlockTexture(buffer);

    SDL_RenderCopy(renderer, buffer, NULL, NULL);

    // flip the backbuffer
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(buffer);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(win);
}

// const int sample_count = 10;
//  CharColor* img_data = (CharColor*)aligned_alloc(32, IMG_WIDTH * IMG_HEIGHT * sizeof(CharColor));

// if constexpr (MULTITHREADING_ENABLED) {

//   auto start_time = system_clock::now();
//   std::array<std::thread, THREAD_COUNT> threads;
//   for (size_t idx = 0; idx < threads.size(); idx++) {

//     constexpr uint32_t chunk_size = IMG_WIDTH * (IMG_HEIGHT / threads.size());
//     threads[idx] = std::thread(render, img_data, chunk_size, idx * chunk_size);
//   }

//   for (size_t idx = 0; idx < threads.size(); idx++) {
//     threads[idx].join();
//   }
//   auto end_time = system_clock::now();
//   auto dur = duration<float>(end_time - start_time);
//   float milli = duration_cast<microseconds>(dur).count() / 1000.f;
//   printf("render time (ms): %f\n", milli);
// } else {
//   double total_milli = 0.0;
//   for (int i = 0; i < sample_count; i++) {
//     auto start_time = system_clock::now();
//     render(img_data, IMG_WIDTH * IMG_HEIGHT, 0);
//     auto end_time = system_clock::now();
//     auto dur = duration<float>(end_time - start_time);
//     float milli = duration_cast<microseconds>(dur).count() / 1000.f;
//     total_milli += milli;
//     printf("sample %d render time (ms): %f\n", i, milli);
//   }
//   printf("total render time (ms): %f\n", total_milli / sample_count);
// }

// stbi_write_png("out.png", IMG_WIDTH, IMG_HEIGHT, 3, img_data, IMG_WIDTH * sizeof(CharColor));
