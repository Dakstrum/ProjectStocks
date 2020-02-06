#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

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
static unsigned int bitmap_cache_size  = 256;
static unsigned int bitmap_cache_index = 0;

static FontCache *font_cache;
static unsigned int font_cache_size  = 256;
static unsigned int font_cache_index = 0;

void InitializeCache() 
{

    bitmap_cache = malloc(sizeof(BitmapCache) * bitmap_cache_size);
    for (unsigned int i = 0; i < bitmap_cache_size; i++)
        bitmap_cache[i].asset_path = NULL;

    font_cache = malloc(sizeof(FontCache) * font_cache_size);
    for (unsigned int i = 0; i < font_cache_size;i++)
        font_cache[i].asset_path = NULL;

}

ALLEGRO_BITMAP *GetBitmapFromCache(char *asset_path) 
{

    if (asset_path == NULL || strcmp(asset_path,"") == 0)
        return NULL;

    for (unsigned int i = 0; i < bitmap_cache_index; i++) {

        if (strcmp(asset_path, bitmap_cache[i].asset_path) == 0)
            return bitmap_cache[i].bitmap;

    }

    return GetNewlyAddedBitmapFromCache(asset_path);

}

void IncreaseCacheSize(unsigned int cache_index, unsigned int *cache_size, int size_of_cache_struct, void *cache)
{

    if (cache_index + 1 < *cache_size)
        return;

    *cache_size += 256;
    cache = realloc(cache, size_of_cache_struct * *(cache_size));
    Log("Increased cache");

}

void IncreaseBitmapCacheSizeIfNeeded() 
{

    IncreaseCacheSize(bitmap_cache_index, &bitmap_cache_size, sizeof(BitmapCache *), (void *)bitmap_cache);

}

ALLEGRO_BITMAP *GetNewlyAddedBitmapFromCache(char *asset_path) 
{

    IncreaseBitmapCacheSizeIfNeeded();

    bitmap_cache[bitmap_cache_index].asset_path = asset_path;
    bitmap_cache[bitmap_cache_index].bitmap     = al_load_bitmap(asset_path);

    bitmap_cache_index++;

    return bitmap_cache[bitmap_cache_index-1].bitmap;
    
}

void IncreaseFontCacheSizeIfNeeded() 
{

    IncreaseCacheSize(font_cache_index, &font_cache_size, sizeof(FontCache *), (void *)font_cache);

}

ALLEGRO_FONT *GetNewlyAddedFontFromCache(char *asset_path, int font_size) 
{

    IncreaseFontCacheSizeIfNeeded();

    font_cache[font_cache_index].font       = al_load_ttf_font(asset_path, font_size, 0);
    font_cache[font_cache_index].font_size  = font_size;
    font_cache[font_cache_index].asset_path = asset_path;

    font_cache_index++;

    return font_cache[font_cache_index-1].font;

}

ALLEGRO_FONT *GetFontFromCache(char *asset_path, int font_size) 
{

    if (asset_path == NULL || font_size == 0)
        return NULL;

    for (size_t i = 0; i < font_cache_index; i++) {

        if (font_cache[i].font_size == font_size && strcmp(font_cache[i].asset_path, asset_path) == 0) 
            return font_cache[i].font;

    }

    return GetNewlyAddedFontFromCache(asset_path, font_size);

}