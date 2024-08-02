#pragma once
#include "types.h"
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <cstdint>

class Camera {
public:
  Vec4 origin = {0, 0, 0.6, 0};
  void register_key_event(SDL_Event e) {

    uint32_t key = e.key.keysym.sym;

    if (e.type == SDL_KEYDOWN) {
      if (key == SDLK_w) {
        velocity.z = -speed;
      }
      if (key == SDLK_s) {
        velocity.z = speed;
      }
      if (key == SDLK_a) {
        velocity.x = -speed;
      }
      if (key == SDLK_d) {
        velocity.x = speed;
      }
    }
    if (e.type == SDL_KEYUP) {
      if (key == SDLK_w) {
        velocity.z = 0;
      }
      if (key == SDLK_s) {
        velocity.z = 0;
      }
      if (key == SDLK_a) {
        velocity.x = 0;
      }
      if (key == SDLK_d) {
        velocity.x = 0;
      }
    }
  }

  void update() {
    origin.x += velocity.x;
    origin.y += velocity.y;
    origin.z += velocity.z;
  }

private:
  Vec3 velocity = {0, 0, 0};
  const float speed = 0.01f;
};
