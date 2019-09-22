#include <stdlib.h>

#include <allegro5/allegro.h>

#include "log.h"

typedef struct Cache {

    char *asset_path;
    ALLEGRO_BITMAP *bitmap;

} Cache;

ALLEGRO_BITMAP *GetNewlyAddedBitmapFromCache(char *asset_path);

static Cache *bitmap_cache;
static unsigned int cache_size  = 256;
static unsigned int cache_index = 0;

void InitializeCache() 
{

    bitmap_cache = malloc(sizeof(Cache *) * cache_size);

}

ALLEGRO_BITMAP *GetBitmapFromCache(char *asset_path) 
{

    for (int i = 0; i < cache_index; i++) {

        if (strcmp(asset_path, bitmap_cache[i].asset_path) == 0)
            return bitmap_cache[i].bitmap;

    }

    return GetNewlyAddedBitmapFromCache(asset_path);

}

void IncreaseCacheSizeIfNeeded() 
{

    if (cache_index + 1 < cache_size)
        return;

    cache_size += 256;
    bitmap_cache = realloc(bitmap_cache, sizeof(Cache *) * cache_size);
    Log("Increase bitmap_cache");

}

ALLEGRO_BITMAP *GetNewlyAddedBitmapFromCache(char *asset_path) 
{

    IncreaseCacheSizeIfNeeded();
    bitmap_cache[cache_index].asset_path = asset_path;
    bitmap_cache[cache_index].bitmap     = al_load_bitmap(asset_path);
    cache_index++;
    return bitmap_cache[cache_index-1].bitmap;


}