#ifndef POPUP_H
#define POPUP_H

#include "drawobject.h"

DrawObject *CreateNewPopup();

void InitPopup(DrawObject *object, const int display_width, const int display_height);
void DrawPopUp(DrawObject *object);
void CleanUpPopUp(DrawObject *object);
void SetPopupText(DrawObject *object, const char *text);
void DisplayPopupOnDrawLayer(char str[50], char *path);

#endif