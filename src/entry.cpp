#include "constants.h"
#include "render.h"
#include "types.h"
#include <cmath>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <immintrin.h>
#include <stb_image_write.h>

int main() {
  CharColor* img_data = (CharColor*)malloc(IMG_WIDTH * IMG_HEIGHT * sizeof(CharColor));
  render(img_data);
  stbi_write_png("out.png", IMG_WIDTH, IMG_HEIGHT, 3, img_data, IMG_WIDTH * sizeof(CharColor));
}
