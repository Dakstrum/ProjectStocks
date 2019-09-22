#ifndef CACHE_H
#define CACHE_H

#include <allegro5/allegro.h>

void InitializeCache();
ALLEGRO_BITMAP *GetBitmapFromCache(char *asset_path);

#endif