#ifndef WINDOW_H
#define WINDOW_H

#include "dbsettings.h"

typedef struct WindowScale {

    float x_scale;
    float y_scale;

} WindowScale;

void Window_Initialize();
void CleanUpDisplay();
void ChangeDisplay(int width, int height);

WindowSettings GetWindowSettings();
WindowScale GetWindowScale();
void Window_Resize(int width, int height);

int Window_Width();
int Window_Height();

void Window_InitVideoBuffer();
void Window_SetVideoBufferAsTarget();
void Window_SetVideoBackBuffer();
void Window_DrawBackBuffer();
float Window_FPS();

#endif