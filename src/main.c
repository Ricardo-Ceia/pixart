#include <stdio.h>
#include <SDL2/SDL.h>
#include "color.h"
#include "renderer.h"
#include "grid.h"
#include "pixel.h"

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window* window = createWindow();
  if (window == NULL) {
    SDL_Quit();
    return 1;
  }

  SDL_Renderer* renderer = createRenderer(window);
  if (renderer == NULL) {
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  SDL_bool running = SDL_TRUE;
  SDL_Event event;
  drawGridOnWindow(renderer);
  while (running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          running = SDL_FALSE;
          break;
        case SDL_MOUSEBUTTONDOWN:
          if (event.button.button == SDL_BUTTON_LEFT) {
            drawPixel(renderer, event.button.x, event.button.y, (Color){255, 0, 0, 255});
          }
          break;
        case SDL_MOUSEMOTION:
          if (event.motion.state & SDL_BUTTON_LMASK) {
            drawPixel(renderer, event.motion.x, event.motion.y, (Color){255, 0, 0, 255});
          }
          break;
      }
    }
    SDL_RenderPresent(renderer);
  }
  return 0;
}
