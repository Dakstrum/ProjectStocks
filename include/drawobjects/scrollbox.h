#ifndef SCROLLBOX_H
#define SCROLLBOX_H

#include "drawobject.h"

#include <allegro5/allegro_font.h>

typedef struct ScrollboxText
{

	short int rx;
	short int ry;
	ALLEGRO_FONT *font;
	unsigned char font_size;
	char *text;

} ScrollboxText;

typedef struct ScrollboxIcon
{

	short int rx;
	short int ry;
	char *icon_path;
	ALLEGRO_BITMAP *icon;

} ScrollboxIcon;

void Scrollbox_Init(DrawObject *object);
void Scrollbox_Draw(DrawObject *object);
void Scrollbox_Clean(DrawObject *object);

DrawObject *Scrollbox_Create();

ScrollboxText Scrollbox_CreateText();
ScrollboxIcon Scrollbox_CreateIcon();

#endif