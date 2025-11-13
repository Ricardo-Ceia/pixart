#include <stdio.h>
#include "renderer.h"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 800

SDL_Window* createWindow() {
  SDL_Window* window = SDL_CreateWindow(
    "Pixel Art Creator",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SDL_WINDOW_SHOWN
  );
  if (window == NULL) {
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
  }
  return window;
}

SDL_Renderer* createRenderer(SDL_Window* window) {
  SDL_Renderer* renderer = SDL_CreateRenderer(
    window,
    -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );
  if (renderer == NULL) {
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
  }
  return renderer;
}

void cleanupRenderer(SDL_Renderer* renderer, SDL_Window* window) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
