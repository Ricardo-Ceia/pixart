#include "grid.h"

#define PIXEL_SIZE 8

void setGrid(Color gridColorArray[SCREEN_WIDTH][SCREEN_HEIGHT]) {
  for (int y = 0; y < SCREEN_HEIGHT; y += PIXEL_SIZE) {
    for (int x = 0; x < SCREEN_WIDTH; x += PIXEL_SIZE) {
      int checkerPattern = ((x / PIXEL_SIZE) + (y / PIXEL_SIZE)) % 2;
      Color pixelColor = checkerPattern == 0 ? 
        (Color){255, 255, 255, 255} : 
        (Color){128, 128, 128, 255};
      
      for (int dy = 0; dy < PIXEL_SIZE; dy++) {
        for (int dx = 0; dx < PIXEL_SIZE; dx++) {
          int screenX = x + dx;
          int screenY = y + dy;
          if (screenX < SCREEN_WIDTH && screenY < SCREEN_HEIGHT) {
            gridColorArray[screenX][screenY] = pixelColor;
          }
        }
      }
    }
  }
}

void renderGrid(SDL_Renderer* renderer, Color gridColorArray[SCREEN_WIDTH][SCREEN_HEIGHT]) {
  for (int y = 0; y < SCREEN_HEIGHT; y += PIXEL_SIZE) {
    for (int x = 0; x < SCREEN_WIDTH; x += PIXEL_SIZE) {
      Color pixelColor = gridColorArray[x][y];
      SDL_SetRenderDrawColor(renderer, pixelColor.r, pixelColor.g, pixelColor.b, pixelColor.a);
      SDL_Rect pixelRect = {x, y, PIXEL_SIZE, PIXEL_SIZE};
      SDL_RenderFillRect(renderer, &pixelRect);
    }
  }
}