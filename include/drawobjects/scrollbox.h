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

void InitScrollbox(DrawObject *object);
void DrawScrollBox(DrawObject *object);
void CleanUpScrollbox(DrawObject *object);

DrawObject *CreateScrollBoxObject();

ScrollboxText CreateScrollboxText();
ScrollboxIcon CreateScrollboxIcon();

#endif