#include "colorpicker.h"
#include <math.h>
#include <stdlib.h>

#define M_PI 3.14159265358979323846
#define PICKER_X 1200
#define PICKER_Y 50
#define PICKER_WIDTH 350
#define PICKER_HEIGHT 250
#define HUE_BAR_HEIGHT 30
#define PREVIEW_SIZE 80

static Color hsvToRgb(float h, float s, float v) {
  float c = v * s;
  float x = c * (1 - fabsf(fmodf(h / 60.0f, 2) - 1));
  float m = v - c;
  
  float r, g, b;
  
  if (h >= 0 && h < 60) {
    r = c; g = x; b = 0;
  } else if (h >= 60 && h < 120) {
    r = x; g = c; b = 0;
  } else if (h >= 120 && h < 180) {
    r = 0; g = c; b = x;
  } else if (h >= 180 && h < 240) {
    r = 0; g = x; b = c;
  } else if (h >= 240 && h < 300) {
    r = x; g = 0; b = c;
  } else {
    r = c; g = 0; b = x;
  }
  
  Color color;
  color.r = (uint8_t)((r + m) * 255);
  color.g = (uint8_t)((g + m) * 255);
  color.b = (uint8_t)((b + m) * 255);
  color.a = 255;
  
  return color;
}

ColorPicker* createColorPicker() {
  ColorPicker* picker = malloc(sizeof(ColorPicker));
  picker->hue = 0.0f;
  picker->saturation = 1.0f;
  picker->value = 1.0f;
  picker->currentColor = hsvToRgb(0, 1, 1);
  picker->pickerRect = (SDL_Rect){PICKER_X, PICKER_Y, PICKER_WIDTH, PICKER_HEIGHT};
  picker->previewRect = (SDL_Rect){PICKER_X + PICKER_WIDTH + 20, PICKER_Y, PREVIEW_SIZE, PREVIEW_SIZE};
  picker->isDragging = 0;
  return picker;
}

void destroyColorPicker(ColorPicker* picker) {
  free(picker);
}

void drawColorPicker(SDL_Renderer* renderer, ColorPicker* picker) {
  SDL_SetRenderDrawColor(renderer, 40, 40, 45, 255);
  SDL_Rect bg = {PICKER_X - 10, PICKER_Y - 10, PICKER_WIDTH + PREVIEW_SIZE + 40, PICKER_HEIGHT + 10};
  SDL_RenderFillRect(renderer, &bg);
  
  int svHeight = PICKER_HEIGHT - HUE_BAR_HEIGHT - 10;
  for (int y = 0; y < svHeight; y++) {
    for (int x = 0; x < PICKER_WIDTH; x++) {
      float s = (float)x / PICKER_WIDTH;
      float v = 1.0f - ((float)y / svHeight);
      Color c = hsvToRgb(picker->hue, s, v);
      SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
      SDL_RenderDrawPoint(renderer, PICKER_X + x, PICKER_Y + y);
    }
  }
  
  int selX = PICKER_X + (int)(picker->saturation * PICKER_WIDTH);
  int selY = PICKER_Y + (int)((1.0f - picker->value) * svHeight);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  for (int i = 0; i < 8; i++) {
    float angle = (i / 8.0f) * 2 * M_PI;
    int x1 = selX + (int)(cos(angle) * 8);
    int y1 = selY + (int)(sin(angle) * 8);
    int x2 = selX + (int)(cos(angle + 0.785f) * 8);
    int y2 = selY + (int)(sin(angle + 0.785f) * 8);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
  }
  
  int hueY = PICKER_Y + svHeight + 10;
  for (int x = 0; x < PICKER_WIDTH; x++) {
    float h = (float)x / PICKER_WIDTH * 360.0f;
    Color c = hsvToRgb(h, 1, 1);
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
    SDL_Rect hueRect = {PICKER_X + x, hueY, 1, HUE_BAR_HEIGHT};
    SDL_RenderFillRect(renderer, &hueRect);
  }
  
  int hueSelX = PICKER_X + (int)(picker->hue / 360.0f * PICKER_WIDTH);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawLine(renderer, hueSelX, hueY, hueSelX, hueY + HUE_BAR_HEIGHT);
  SDL_RenderDrawLine(renderer, hueSelX - 1, hueY, hueSelX - 1, hueY + HUE_BAR_HEIGHT);
  SDL_RenderDrawLine(renderer, hueSelX + 1, hueY, hueSelX + 1, hueY + HUE_BAR_HEIGHT);
  
  SDL_SetRenderDrawColor(renderer, picker->currentColor.r, picker->currentColor.g, 
                         picker->currentColor.b, picker->currentColor.a);
  SDL_RenderFillRect(renderer, &picker->previewRect);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawRect(renderer, &picker->previewRect);
}

void handleColorPickerInput(ColorPicker* picker, int mouseX, int mouseY, int scrollDelta) {
  if (!picker->isDragging && scrollDelta == 0) {
    return; 
  }
  
  int svHeight = PICKER_HEIGHT - HUE_BAR_HEIGHT - 10;
  int hueY = PICKER_Y + svHeight + 10;
  
  if (mouseX >= PICKER_X && mouseX < PICKER_X + PICKER_WIDTH &&
      mouseY >= PICKER_Y && mouseY < PICKER_Y + svHeight) {
    picker->saturation = (float)(mouseX - PICKER_X) / PICKER_WIDTH;
    picker->value = 1.0f - ((float)(mouseY - PICKER_Y) / svHeight);
    picker->saturation = fmaxf(0.0f, fminf(1.0f, picker->saturation));
    picker->value = fmaxf(0.0f, fminf(1.0f, picker->value));
    picker->currentColor = hsvToRgb(picker->hue, picker->saturation, picker->value);
  }
  if (mouseX >= PICKER_X && mouseX < PICKER_X + PICKER_WIDTH &&
      mouseY >= hueY && mouseY < hueY + HUE_BAR_HEIGHT) {
    picker->hue = ((float)(mouseX - PICKER_X) / PICKER_WIDTH) * 360.0f;
    picker->hue = fmaxf(0.0f, fminf(360.0f, picker->hue));
    picker->currentColor = hsvToRgb(picker->hue, picker->saturation, picker->value);
  }
  
  if (scrollDelta != 0) {
    picker->hue += scrollDelta * 5.0f;
    if (picker->hue < 0) picker->hue += 360;
    if (picker->hue >= 360) picker->hue -= 360;
    picker->currentColor = hsvToRgb(picker->hue, picker->saturation, picker->value);
  }
}

void setColorPickerDragging(ColorPicker* picker, int isDragging) {
  picker->isDragging = isDragging;
}

int isMouseOverColorPicker(ColorPicker* picker, int mouseX, int mouseY) {
  return (mouseX >= PICKER_X - 10 && mouseX <= PICKER_X + PICKER_WIDTH + PREVIEW_SIZE + 30 && 
          mouseY >= PICKER_Y - 10 && mouseY <= PICKER_Y + PICKER_HEIGHT);
}

Color getSelectedColor(ColorPicker* picker) {
  return picker->currentColor;
}
