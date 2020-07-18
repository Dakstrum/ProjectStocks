#ifndef DRAW_H
#define DRAW_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

void RefreshDrawScale();
void DrawGeneric(ALLEGRO_BITMAP *bitmap, float x, float y);
void DrawGenericTinted(ALLEGRO_BITMAP *bitmap, float x, float y, ALLEGRO_COLOR tint);
void DrawGenericWithWidth(ALLEGRO_BITMAP *bitmap, float x, float y, float width, float height);
void DrawBackBuffer(ALLEGRO_BITMAP *bitmap);
void DrawFrameTiming(double dt);
void Draw_Initialize();

#endif