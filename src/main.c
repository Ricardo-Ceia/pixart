#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "color.h"
#include "renderer.h"
#include "grid.h"
#include "pixel.h"
#include "fps.h"
#include "colorpicker.h"

Color gridColorArray[SCREEN_WIDTH][SCREEN_HEIGHT];

static void initializeGrid(Color gridColorArray[SCREEN_WIDTH][SCREEN_HEIGHT]) {
  for (int y = 0; y < SCREEN_HEIGHT; y++) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
      gridColorArray[x][y] = (Color){0, 0, 0, 255};
    }
  }
  setGrid(gridColorArray);
}

static void handleMouseButtonDown(SDL_MouseButtonEvent button, ColorPicker* colorPicker, 
                                  Color gridColorArray[SCREEN_WIDTH][SCREEN_HEIGHT]) {
  if (button.button != SDL_BUTTON_LEFT) return;
  
  if (isMouseOverColorPicker(colorPicker, button.x, button.y)) {
    setColorPickerDragging(colorPicker, 1);
    handleColorPickerInput(colorPicker, button.x, button.y, 0);
  } else {
    drawPixelToGrid(gridColorArray, button.x, button.y, getSelectedColor(colorPicker));
  }
}

static void handleMouseMotion(SDL_MouseMotionEvent motion, ColorPicker* colorPicker,
                              Color gridColorArray[SCREEN_WIDTH][SCREEN_HEIGHT]) {
  if (!(motion.state & SDL_BUTTON_LMASK)) return;
  
  if (isMouseOverColorPicker(colorPicker, motion.x, motion.y)) {
    if (colorPicker->isDragging) {
      handleColorPickerInput(colorPicker, motion.x, motion.y, 0);
    }
  } else {
    drawPixelToGrid(gridColorArray, motion.x, motion.y, getSelectedColor(colorPicker));
  }
}

static void handleMouseButtonUp(SDL_MouseButtonEvent button, ColorPicker* colorPicker) {
  if (button.button == SDL_BUTTON_LEFT) {
    setColorPickerDragging(colorPicker, 0);
  }
}

static void processEvents(SDL_bool* running, ColorPicker* colorPicker,
                          Color gridColorArray[SCREEN_WIDTH][SCREEN_HEIGHT]) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        *running = SDL_FALSE;
        break;
      case SDL_MOUSEBUTTONDOWN:
        handleMouseButtonDown(event.button, colorPicker, gridColorArray);
        break;
      case SDL_MOUSEBUTTONUP:
        handleMouseButtonUp(event.button, colorPicker);
        break;
      case SDL_MOUSEMOTION:
        handleMouseMotion(event.motion, colorPicker, gridColorArray);
        break;
    }
  }
}

static void updateFrame(SDL_Renderer* renderer, ColorPicker* colorPicker,
                        Color gridColorArray[SCREEN_WIDTH][SCREEN_HEIGHT],
                        TTF_Font* font, uint32_t frameTime) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  
  drawColorPicker(renderer, colorPicker, gridColorArray);
  renderGrid(renderer, gridColorArray);
  drawFps(renderer, font, currentFps(frameTime));
  
  SDL_RenderPresent(renderer);
}

static void gameLoop(SDL_Renderer* renderer, ColorPicker* colorPicker,
                     Color gridColorArray[SCREEN_WIDTH][SCREEN_HEIGHT], TTF_Font* font) {
  SDL_bool running = SDL_TRUE;
  uint32_t frameStart, frameTime;
  
  while (running) {
    frameStart = SDL_GetTicks();
    
    processEvents(&running, colorPicker, gridColorArray);
    updateFrame(renderer, colorPicker, gridColorArray, font, frameTime);
    
    frameTime = SDL_GetTicks() - frameStart;
  }
}

int main(void) {
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
  initializeGrid(gridColorArray);
  gameLoop(renderer, colorPicker, gridColorArray, font);
  
  destroyColorPicker(colorPicker);
  if (font) TTF_CloseFont(font);
  cleanupRenderer(renderer, window);
  TTF_Quit();
  
  return 0;
}

