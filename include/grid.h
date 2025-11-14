#ifndef GRID_H
#define GRID_H

#include <SDL2/SDL.h>
#include "color.h"
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 800

void setGrid(Color gridColorArray[SCREEN_WIDTH][SCREEN_HEIGHT]);
void renderGrid(SDL_Renderer* renderer, Color gridColorArray[SCREEN_WIDTH][SCREEN_HEIGHT]);

#endif
