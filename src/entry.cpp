#include "constants.h"
#include "render.h"

int main() {
  if constexpr (ACTIVE_RENDER_MODE == RENDER_MODE::REAL_TIME) {
    render_realtime();
  } else if constexpr (ACTIVE_RENDER_MODE == RENDER_MODE::PNG) {
    render_png();
  }
}
