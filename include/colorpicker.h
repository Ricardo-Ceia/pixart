#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <SDL2/SDL.h>
#include "color.h"
#include "grid.h"

typedef struct {
  float hue;        // 0-360
  float saturation; // 0-1
  float value;      // 0-1
  Color currentColor;
  SDL_Rect pickerRect;
  SDL_Rect previewRect;
  int isDragging;
} ColorPicker;

ColorPicker* createColorPicker();
void destroyColorPicker(ColorPicker* picker);
void drawColorPicker(SDL_Renderer* renderer, ColorPicker* picker, Color gridColorArray[SCREEN_WIDTH][SCREEN_HEIGHT]);
void handleColorPickerInput(ColorPicker* picker, int mouseX, int mouseY, int scrollDelta);
void setColorPickerDragging(ColorPicker* picker, int isDragging);
int isMouseOverColorPicker(ColorPicker* picker, int mouseX, int mouseY);
Color getSelectedColor(ColorPicker* picker);

#endif
