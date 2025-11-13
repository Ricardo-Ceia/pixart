#ifndef FPS_H
#define FPS_H

#include <time.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

float currentFps(uint32_t timeDelta);
void drawFps(SDL_Renderer* renderer, TTF_Font* font, float fps);
#endif
