#ifndef POPUP_H
#define POPUP_H

#include "drawobject.h"

void InitPopup(DrawObject *object, const int display_width, const int display_height);
void DrawPopUp(DrawObject *object);
void CleanUpPopUp(DrawObject *object);

#endif