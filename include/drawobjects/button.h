#ifndef BUTTON_H
#define BUTTON_H

#include "drawobject.h"

void InitButton(DrawObject *object);
void DrawButton(DrawObject *object);
void CleanUpButton(DrawObject *object);

void SetButtonTint(DrawObject *object, RGBA *color);
void ClearButtonTint(DrawObject *object);

#endif