#include "globals.h"
#include "render.h"

int main() {
  if constexpr (global::active_render_mode == RenderMode::real_time) {
    render_realtime();
  } else if constexpr (global::active_render_mode == RenderMode::png) {
    render_png();
  }
}
