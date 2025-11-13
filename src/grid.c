#include "grid.h"
#include "color.h"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 800
#define PIXEL_SIZE 8

void drawGridOnWindow(SDL_Renderer* renderer) {
  Color pixelColor;
  for (int y = 0; y < SCREEN_HEIGHT; y += PIXEL_SIZE) {
    for (int x = 0; x < SCREEN_WIDTH; x += PIXEL_SIZE) {
      if (((x / PIXEL_SIZE) + (y / PIXEL_SIZE)) % 2 == 0) {
        pixelColor = (Color){255, 255, 255, 255};
      } else {
        pixelColor = (Color){128, 128, 128, 255};
      }
      SDL_SetRenderDrawColor(renderer, pixelColor.r, pixelColor.g, pixelColor.b, pixelColor.a);
      SDL_Rect pixel_rect = {x, y, PIXEL_SIZE, PIXEL_SIZE};
      SDL_RenderFillRect(renderer, &pixel_rect);
    }
  }
}
