#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "color.h"
#include "renderer.h"
#include "grid.h"
#include "pixel.h"
#include <fps.h>
#include "colorpicker.h"

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }
  
  TTF_Init();

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

  TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);
  if (font == NULL) {
    printf("Failed to load font: %s\n", TTF_GetError());
  }
  
  ColorPicker* colorPicker = createColorPicker();
  SDL_bool running = SDL_TRUE;
  SDL_Event event;

  uint32_t frameStart;
  drawGridOnWindow(renderer);
  while (running) {
    frameStart = SDL_GetTicks();
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          running = SDL_FALSE;
          break;
        case SDL_MOUSEBUTTONDOWN:
          if (event.button.button == SDL_BUTTON_LEFT) {
            if (isMouseOverColorPicker(colorPicker, event.button.x, event.button.y)) {
              setColorPickerDragging(colorPicker, 1);
              handleColorPickerInput(colorPicker, event.button.x, event.button.y, 0);
            } else {
              Color drawColor = getSelectedColor(colorPicker);
              drawPixel(renderer, event.button.x, event.button.y, drawColor);
            }
          }
          break;
        case SDL_MOUSEBUTTONUP:
          if (event.button.button == SDL_BUTTON_LEFT) {
            setColorPickerDragging(colorPicker, 0);
          }
          break;
        case SDL_MOUSEMOTION:
          if (event.motion.state & SDL_BUTTON_LMASK) {
            if (isMouseOverColorPicker(colorPicker, event.motion.x, event.motion.y)) {
              if (colorPicker->isDragging) {
                handleColorPickerInput(colorPicker, event.motion.x, event.motion.y, 0);
              }
            } else {
              Color drawColor = getSelectedColor(colorPicker);
              drawPixel(renderer, event.motion.x, event.motion.y, drawColor);
            }
          }
          break;
      }
    }
    SDL_RenderPresent(renderer);
    drawColorPicker(renderer,colorPicker);
    float frames = currentFps(SDL_GetTicks() - frameStart);
    drawFps(renderer,font,frames);
  }
  return 0;
}

