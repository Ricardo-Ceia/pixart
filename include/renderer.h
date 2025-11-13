#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>

SDL_Window* createWindow();
SDL_Renderer* createRenderer(SDL_Window* window);
void cleanupRenderer(SDL_Renderer* renderer, SDL_Window* window);

#endif
