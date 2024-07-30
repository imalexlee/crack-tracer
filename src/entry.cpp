#include "camera.h"
#include "constants.h"
#include "render.h"
#include <SDL2/SDL.h>
#include <array>
#include <future>

using namespace std::chrono;

int main() {
  SDL_Window* win = NULL;
  SDL_Renderer* renderer = NULL;
  CharColor* img_data = (CharColor*)aligned_alloc(32, IMG_WIDTH * IMG_HEIGHT * sizeof(CharColor));
  Camera cam;

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    return 1;

  win = SDL_CreateWindow("Crack Tracer", 100, 100, IMG_WIDTH, IMG_HEIGHT, 0);
  renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

  SDL_Texture* buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24,
                                          SDL_TEXTUREACCESS_STREAMING, IMG_WIDTH, IMG_HEIGHT);

  int pitch = IMG_WIDTH * sizeof(CharColor);
  std::array<std::future<void>, THREAD_COUNT> futures;
  constexpr uint32_t chunk_size = IMG_WIDTH * (IMG_HEIGHT / THREAD_COUNT);

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
}
