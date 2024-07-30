#include "camera.h"
#include "constants.h"
#include "render.h"
#include <SDL2/SDL.h>
#include <array>
#include <future>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <immintrin.h>
#include <stb_image_write.h>

using namespace std::chrono;

int main() {

  CharColor* img_data = (CharColor*)aligned_alloc(32, IMG_WIDTH * IMG_HEIGHT * sizeof(CharColor));
  std::array<std::future<void>, THREAD_COUNT> futures;
  constexpr uint32_t chunk_size = IMG_WIDTH * (IMG_HEIGHT / THREAD_COUNT);
  Camera cam;

  if constexpr (ACTIVE_RENDER_MODE == RENDER_MODE::REAL_TIME) {
    SDL_Window* win = NULL;
    SDL_Renderer* renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
      return 1;

    win = SDL_CreateWindow("Crack Tracer", 100, 100, IMG_WIDTH, IMG_HEIGHT, 0);
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24,
                                            SDL_TEXTUREACCESS_STREAMING, IMG_WIDTH, IMG_HEIGHT);

    int pitch = IMG_WIDTH * sizeof(CharColor);

    while (true) {

      SDL_Event e;
      if (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
          break;
        }
        cam.register_key_event(e);
      }

      cam.update();

      SDL_LockTexture(buffer, NULL, (void**)(&img_data), &pitch);

      for (size_t idx = 0; idx < THREAD_COUNT; idx++) {
        futures[idx] = std::async(std::launch::async, render, img_data, cam.origin, chunk_size,
                                  idx * chunk_size);
      }

      for (size_t idx = 0; idx < THREAD_COUNT; idx++) {
        futures[idx].get();
      }

      SDL_UnlockTexture(buffer);

      SDL_RenderCopy(renderer, buffer, NULL, NULL);

      // flip the backbuffer
      SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
  } else if constexpr (ACTIVE_RENDER_MODE == RENDER_MODE::PNG) {
    auto start_time = system_clock::now();
    for (size_t idx = 0; idx < THREAD_COUNT; idx++) {
      futures[idx] = std::async(std::launch::async, render, img_data, cam.origin, chunk_size,
                                idx * chunk_size);
    }

    for (size_t idx = 0; idx < THREAD_COUNT; idx++) {
      futures[idx].get();
    }
    auto end_time = system_clock::now();
    auto dur = duration<float>(end_time - start_time);
    float milli = duration_cast<microseconds>(dur).count() / 1000.f;
    printf("render time (ms): %f\n", milli);
    stbi_write_png("out.png", IMG_WIDTH, IMG_HEIGHT, 3, img_data, IMG_WIDTH * sizeof(CharColor));
  }
}
