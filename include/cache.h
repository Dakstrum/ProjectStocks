#ifndef CACHE_H
#define CACHE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

void InitializeCache();
ALLEGRO_BITMAP *GetBitmapFromCache(char *asset_path);
ALLEGRO_FONT *GetFontFromCache(char *asset_path, int font_size);

#endif