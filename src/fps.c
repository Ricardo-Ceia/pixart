#include "fps.h"


float currentFps(uint32_t timeDelta){
  return 1000.0f/timeDelta;
}

void drawFps(SDL_Renderer* renderer, TTF_Font* font, float fps) {
  if (font == NULL) return;

  char fpsText[32];
  snprintf(fpsText, sizeof(fpsText), "FPS: %.1f", fps);

  SDL_Color textColor = {255, 255, 255, 255};

  SDL_Surface* surface = TTF_RenderText_Solid(font, fpsText, textColor);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect rect = {15, 15, surface->w + 10, surface->h + 10};
  
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
  SDL_RenderFillRect(renderer, &rect);
  
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawRect(renderer, &rect);
  
  SDL_Rect textRect = {20, 20, surface->w, surface->h};
  SDL_RenderCopy(renderer, texture, NULL, &textRect);

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}
