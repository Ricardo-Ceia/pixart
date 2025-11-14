#include "pixel.h"

#define PIXEL_SIZE 8

void drawPixelToGrid(Color gridColorArray[SCREEN_WIDTH][SCREEN_HEIGHT], 
                     uint16_t mouseX, uint16_t mouseY, Color color) {
  uint16_t pixelGridX = mouseX / PIXEL_SIZE;
  uint16_t pixelGridY = mouseY / PIXEL_SIZE;
  
  uint16_t pixelStartX = pixelGridX * PIXEL_SIZE;
  uint16_t pixelStartY = pixelGridY * PIXEL_SIZE;
  
  for (int y = 0; y < PIXEL_SIZE; y++) {
    for (int x = 0; x < PIXEL_SIZE; x++) {
      int screenX = pixelStartX + x;
      int screenY = pixelStartY + y;
      if (screenX < SCREEN_WIDTH && screenY < SCREEN_HEIGHT) {
        gridColorArray[screenX][screenY] = color;
      }
    }
  }
}
