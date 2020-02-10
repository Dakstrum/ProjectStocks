#ifndef TEXT_H
#define TEXT_H

#include "drawobject.h"

void InitText(DrawObject *object);
void CleanUpText(DrawObject *object);
void DrawText(DrawObject *object);
void SetTextContent(DrawObject *object, const char *str, ...);

#endif