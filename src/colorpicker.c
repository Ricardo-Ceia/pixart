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

ColorPicker* createColorPicker(void) {
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

static void drawLine(Color gridColorArray[SCREEN_WIDTH][SCREEN_HEIGHT], 
                     int x1, int y1, int x2, int y2, Color color) {
  int dx = x2 - x1;
  int dy = y2 - y1;
  int steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);
  
  for (int step = 0; step <= steps; step++) {
    int x = x1 + (dx * step) / (steps + 1);
    int y = y1 + (dy * step) / (steps + 1);
    
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
      gridColorArray[x][y] = color;
    }
  }
}

static void drawSelectionCircle(Color gridColorArray[SCREEN_WIDTH][SCREEN_HEIGHT], int centerX, int centerY) {
  Color white = {255, 255, 255, 255};
  int radius = 8;
  
  for (int i = 0; i < 8; i++) {
    float angle = (i / 8.0f) * 2 * M_PI;
    int x1 = centerX + (int)(cosf(angle) * radius);
    int y1 = centerY + (int)(sinf(angle) * radius);
    int x2 = centerX + (int)(cosf(angle + 0.785f) * radius);
    int y2 = centerY + (int)(sinf(angle + 0.785f) * radius);
    
    drawLine(gridColorArray, x1, y1, x2, y2, white);
  }
}

static void drawSVSquare(Color gridColorArray[SCREEN_WIDTH][SCREEN_HEIGHT], ColorPicker* picker) {
  int svHeight = PICKER_HEIGHT - HUE_BAR_HEIGHT - 10;
  
  for (int y = 0; y < svHeight; y++) {
    for (int x = 0; x < PICKER_WIDTH; x++) {
      int screenX = PICKER_X + x;
      int screenY = PICKER_Y + y;
      if (screenX >= 0 && screenX < SCREEN_WIDTH && screenY >= 0 && screenY < SCREEN_HEIGHT) {
        float saturation = (float)x / PICKER_WIDTH;
        float value = 1.0f - ((float)y / svHeight);
        Color color = hsvToRgb(picker->hue, saturation, value);
        gridColorArray[screenX][screenY] = color;
      }
    }
  }
  
  int selX = PICKER_X + (int)(picker->saturation * PICKER_WIDTH);
  int selY = PICKER_Y + (int)((1.0f - picker->value) * svHeight);
  drawSelectionCircle(gridColorArray, selX, selY);
}

static void drawHueBar(Color gridColorArray[SCREEN_WIDTH][SCREEN_HEIGHT], ColorPicker* picker) {
  int svHeight = PICKER_HEIGHT - HUE_BAR_HEIGHT - 10;
  int hueY = PICKER_Y + svHeight + 10;
  
  for (int x = 0; x < PICKER_WIDTH; x++) {
    float hue = ((float)x / PICKER_WIDTH) * 360.0f;
    Color color = hsvToRgb(hue, 1.0f, 1.0f);
    for (int y = 0; y < HUE_BAR_HEIGHT; y++) {
      int screenX = PICKER_X + x;
      int screenY = hueY + y;
      if (screenX >= 0 && screenX < SCREEN_WIDTH && screenY >= 0 && screenY < SCREEN_HEIGHT) {
        gridColorArray[screenX][screenY] = color;
      }
    }
  }
  
  int hueSelX = PICKER_X + (int)(picker->hue / 360.0f * PICKER_WIDTH);
  Color white = {255, 255, 255, 255};
  for (int y = 0; y < HUE_BAR_HEIGHT; y++) {
    for (int xOffset = -1; xOffset <= 1; xOffset++) {
      int x = hueSelX + xOffset;
      int screenY = hueY + y;
      if (x >= 0 && x < SCREEN_WIDTH && screenY >= 0 && screenY < SCREEN_HEIGHT) {
        gridColorArray[x][screenY] = white;
      }
    }
  }
}

static void drawPreview(Color gridColorArray[SCREEN_WIDTH][SCREEN_HEIGHT], ColorPicker* picker) {
  int previewX = PICKER_X + PICKER_WIDTH + 20;
  int previewY = PICKER_Y;
  
  for (int x = 0; x < PREVIEW_SIZE; x++) {
    for (int y = 0; y < PREVIEW_SIZE; y++) {
      int screenX = previewX + x;
      int screenY = previewY + y;
      if (screenX >= 0 && screenX < SCREEN_WIDTH && screenY >= 0 && screenY < SCREEN_HEIGHT) {
        gridColorArray[screenX][screenY] = picker->currentColor;
      }
    }
  }
  
  Color borderColor = {200, 200, 200, 255};
  for (int i = 0; i < PREVIEW_SIZE; i++) {
    if (previewX + i >= 0 && previewX + i < SCREEN_WIDTH) {
      if (previewY >= 0 && previewY < SCREEN_HEIGHT) {
        gridColorArray[previewX + i][previewY] = borderColor;
      }
      if (previewY + PREVIEW_SIZE - 1 >= 0 && previewY + PREVIEW_SIZE - 1 < SCREEN_HEIGHT) {
        gridColorArray[previewX + i][previewY + PREVIEW_SIZE - 1] = borderColor;
      }
    }
    if (previewY + i >= 0 && previewY + i < SCREEN_HEIGHT) {
      if (previewX >= 0 && previewX < SCREEN_WIDTH) {
        gridColorArray[previewX][previewY + i] = borderColor;
      }
      if (previewX + PREVIEW_SIZE - 1 >= 0 && previewX + PREVIEW_SIZE - 1 < SCREEN_WIDTH) {
        gridColorArray[previewX + PREVIEW_SIZE - 1][previewY + i] = borderColor;
      }
    }
  }
}

void drawColorPicker(SDL_Renderer* renderer, ColorPicker* picker, 
                     Color gridColorArray[SCREEN_WIDTH][SCREEN_HEIGHT]) {
  (void)renderer;
  drawSVSquare(gridColorArray, picker);
  drawHueBar(gridColorArray, picker);
  drawPreview(gridColorArray, picker);
}

static void updateSVSelection(ColorPicker* picker, int mouseX, int mouseY) {
  int svHeight = PICKER_HEIGHT - HUE_BAR_HEIGHT - 10;
  
  if (mouseX >= PICKER_X && mouseX < PICKER_X + PICKER_WIDTH &&
      mouseY >= PICKER_Y && mouseY < PICKER_Y + svHeight) {
    picker->saturation = (float)(mouseX - PICKER_X) / PICKER_WIDTH;
    picker->value = 1.0f - ((float)(mouseY - PICKER_Y) / svHeight);
    picker->saturation = fmaxf(0.0f, fminf(1.0f, picker->saturation));
    picker->value = fmaxf(0.0f, fminf(1.0f, picker->value));
    picker->currentColor = hsvToRgb(picker->hue, picker->saturation, picker->value);
  }
}

static void updateHueSelection(ColorPicker* picker, int mouseX, int mouseY) {
  int svHeight = PICKER_HEIGHT - HUE_BAR_HEIGHT - 10;
  int hueY = PICKER_Y + svHeight + 10;
  
  if (mouseX >= PICKER_X && mouseX < PICKER_X + PICKER_WIDTH &&
      mouseY >= hueY && mouseY < hueY + HUE_BAR_HEIGHT) {
    picker->hue = ((float)(mouseX - PICKER_X) / PICKER_WIDTH) * 360.0f;
    picker->hue = fmaxf(0.0f, fminf(360.0f, picker->hue));
    picker->currentColor = hsvToRgb(picker->hue, picker->saturation, picker->value);
  }
}

static void updateHueFromScroll(ColorPicker* picker, int scrollDelta) {
  picker->hue += scrollDelta * 5.0f;
  if (picker->hue < 0) picker->hue += 360;
  if (picker->hue >= 360) picker->hue -= 360;
  picker->currentColor = hsvToRgb(picker->hue, picker->saturation, picker->value);
}

void handleColorPickerInput(ColorPicker* picker, int mouseX, int mouseY, int scrollDelta) {
  if (!picker->isDragging && scrollDelta == 0) return;
  
  updateSVSelection(picker, mouseX, mouseY);
  updateHueSelection(picker, mouseX, mouseY);
  
  if (scrollDelta != 0) {
    updateHueFromScroll(picker, scrollDelta);
  }
}

void setColorPickerDragging(ColorPicker* picker, int isDragging) {
  picker->isDragging = isDragging;
}

int isMouseOverColorPicker(ColorPicker* picker, int mouseX, int mouseY) {
  (void)picker;
  return (mouseX >= PICKER_X - 10 && mouseX <= PICKER_X + PICKER_WIDTH + PREVIEW_SIZE + 30 && 
          mouseY >= PICKER_Y - 10 && mouseY <= PICKER_Y + PICKER_HEIGHT);
}

Color getSelectedColor(ColorPicker* picker) {
  return picker->currentColor;
}
