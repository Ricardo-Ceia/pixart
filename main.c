#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 800
#define PIXEL_SIZE 8

typedef struct{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
}Color;

void drawGridOnWindow(SDL_Renderer* renderer);
void drawPixel(SDL_Renderer* renderer,uint16_t mouseX,uint16_t mouseY, Color color);



int main(){
  if (SDL_Init(SDL_INIT_VIDEO) != 0){
    printf("SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window* window = SDL_CreateWindow(
    "Pixel Art Creator",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SDL_WINDOW_SHOWN
  );

  if (window == NULL){
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(
    window,
    -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );

  if (renderer == NULL){
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
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
          if (event.button.button==SDL_BUTTON_LEFT) {
            uint16_t mouseX = event.button.x;
            uint16_t  mouseY = event.button.y;
            drawPixel(renderer,mouseX,mouseY,(Color){255,0,0,255});
          }
          break;
      }
    }
    SDL_RenderPresent(renderer);
  }
  return 0;
}

void drawGridOnWindow(SDL_Renderer* renderer){
  Color pixelColor;
  bool is_white = true;
  for (int y=0;y<SCREEN_HEIGHT;y+=PIXEL_SIZE){
    for (int x=0;x<SCREEN_WIDTH;x+=PIXEL_SIZE){
      if(((x/PIXEL_SIZE)+(y/PIXEL_SIZE))%2==0){
        pixelColor = (Color){255,255,255,255};
      }
      else{
        pixelColor = (Color){128,128,128,255};
      }
      SDL_SetRenderDrawColor(renderer, pixelColor.r, pixelColor.g, pixelColor.b, pixelColor.a);
      SDL_Rect pixel_rect = {x, y, PIXEL_SIZE, PIXEL_SIZE};
      SDL_RenderFillRect(renderer, &pixel_rect);
    }
  }
}


void drawPixel(SDL_Renderer* renderer,uint16_t mouseX,uint16_t mouseY,Color color){
  uint16_t pixelGridX = mouseX / PIXEL_SIZE;
  uint16_t pixelGridY = mouseY / PIXEL_SIZE;
  
  uint16_t pixelStartX = pixelGridX * PIXEL_SIZE;
  uint16_t pixelStartY = pixelGridY * PIXEL_SIZE;
  
  SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
  SDL_Rect  pixel_rect = {pixelStartX,pixelStartY,PIXEL_SIZE,PIXEL_SIZE};
  SDL_RenderFillRect(renderer,&pixel_rect);
}
