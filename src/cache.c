#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "log.h"

typedef struct BitmapCache {

    char *asset_path;
    ALLEGRO_BITMAP *bitmap;

} BitmapCache;

typedef struct FontCache {

    char *asset_path;
    ALLEGRO_FONT *font;
    int font_size;


} FontCache;

ALLEGRO_BITMAP *GetNewlyAddedBitmapFromCache(char *asset_path);

static BitmapCache *bitmap_cache;
static FontCache   *font_cache;
static unsigned int bitmap_cache_size  = 256;
static unsigned int bitmap_cache_index = 0;

void InitializeCache() 
{

    bitmap_cache = malloc(sizeof(BitmapCache) * bitmap_cache_size);
    for (int i = 0; i < bitmap_cache_size; i++)
        bitmap_cache[i].asset_path = NULL;
}

ALLEGRO_BITMAP *GetBitmapFromCache(char *asset_path) 
{

    if (asset_path == NULL || strcmp(asset_path,"") == 0)
        return NULL;

    for (int i = 0; i < bitmap_cache_index; i++) {

        if (strcmp(asset_path, bitmap_cache[i].asset_path) == 0)
            return bitmap_cache[i].bitmap;

    }

    return GetNewlyAddedBitmapFromCache(asset_path);

}

void IncreaseCacheSizeIfNeeded() 
{

    if (bitmap_cache_index + 1 < bitmap_cache_size)
        return;

    bitmap_cache_size += 256;
    bitmap_cache = realloc(bitmap_cache, sizeof(BitmapCache *) * bitmap_cache_size);
    Log("Increase bitmap_cache");

}

ALLEGRO_BITMAP *GetNewlyAddedBitmapFromCache(char *asset_path) 
{

    IncreaseCacheSizeIfNeeded();
    bitmap_cache[bitmap_cache_index].asset_path = asset_path;
    bitmap_cache[bitmap_cache_index].bitmap     = al_load_bitmap(asset_path);
    bitmap_cache_index++;

    return bitmap_cache[bitmap_cache_index-1].bitmap;
    
}