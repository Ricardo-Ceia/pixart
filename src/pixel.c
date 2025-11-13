#include "pixel.h"

#define PIXEL_SIZE 8

void drawPixel(SDL_Renderer* renderer, uint16_t mouseX, uint16_t mouseY, Color color) {
  uint16_t pixelGridX = mouseX / PIXEL_SIZE;
  uint16_t pixelGridY = mouseY / PIXEL_SIZE;
  
  uint16_t pixelStartX = pixelGridX * PIXEL_SIZE;
  uint16_t pixelStartY = pixelGridY * PIXEL_SIZE;
  
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_Rect pixel_rect = {pixelStartX, pixelStartY, PIXEL_SIZE, PIXEL_SIZE};
  SDL_RenderFillRect(renderer, &pixel_rect);
}
