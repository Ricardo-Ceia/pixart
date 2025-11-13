#ifndef PIXEL_H
#define PIXEL_H

#include <SDL2/SDL.h>
#include "color.h"

void drawPixel(SDL_Renderer* renderer, uint16_t mouseX, uint16_t mouseY, Color color);

#endif
