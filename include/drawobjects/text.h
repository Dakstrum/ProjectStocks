#ifndef TEXT_H
#define TEXT_H

#include "drawobject.h"

void InitText(DrawObject *object);
void CleanUpText(DrawObject *object);
void DrawText(DrawObject *object);
void SetTextContent(DrawObject *object, const char *str, ...);
void SetTextContentWithCommaFormat(DrawObject *object, const char *str, ...);
void SetTextColor(DrawObject *object, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
DrawObject *Text_Create();

#endif