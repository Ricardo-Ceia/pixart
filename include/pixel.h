#ifndef PIXEL_H
#define PIXEL_H

#include <stdint.h>
#include "color.h"
#include "grid.h"

void drawPixelToGrid(Color gridColorArray[SCREEN_WIDTH][SCREEN_HEIGHT], 
                     uint16_t mouseX, uint16_t mouseY, Color color);

#endif
