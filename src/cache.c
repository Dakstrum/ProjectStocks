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
static unsigned short int bitmap_cache_size  = 256;
static unsigned short int bitmap_cache_index = 0;

static FontCache *font_cache;
static unsigned short int font_cache_size  = 256;
static unsigned short int font_cache_index = 0;

void InitializeCache() 
{

    bitmap_cache = malloc(sizeof(BitmapCache) * bitmap_cache_size);
    for (unsigned short int i = 0; i < bitmap_cache_size; i++)
        bitmap_cache[i].asset_path = NULL;

    font_cache = malloc(sizeof(FontCache) * font_cache_size);
    for (unsigned short int i = 0; i < font_cache_size;i++)
        font_cache[i].asset_path = NULL;

}

void IncreaseCacheSize(unsigned short int cache_index, unsigned short int *cache_size, int size_of_cache_struct, void *cache)
{

    if (cache_index + 1 < *cache_size)
        return;

    *cache_size += 256;
    cache = realloc(cache, size_of_cache_struct * *(cache_size));
    Log("Increased cache");

}


ALLEGRO_BITMAP *GetNewlyAddedBitmapFromCache(char *asset_path) 
{

    IncreaseCacheSize(bitmap_cache_index, &bitmap_cache_size, sizeof(BitmapCache *), (void *)bitmap_cache);;

    bitmap_cache[bitmap_cache_index].asset_path = asset_path;
    bitmap_cache[bitmap_cache_index].bitmap     = al_load_bitmap(asset_path);

    bitmap_cache_index++;

    return bitmap_cache[bitmap_cache_index-1].bitmap;
    
}


ALLEGRO_BITMAP *GetBitmapFromCache(char *asset_path) 
{

    if (asset_path == NULL || strcmp(asset_path,"") == 0)
        return NULL;

    for (unsigned short int i = 0; i < bitmap_cache_index; i++) {

        if (strcmp(asset_path, bitmap_cache[i].asset_path) == 0)
            return bitmap_cache[i].bitmap;

    }

    //LogF("Returning new bitmap %s", asset_path);
    return GetNewlyAddedBitmapFromCache(asset_path);

}

ALLEGRO_FONT *GetNewlyAddedFontFromCache(char *asset_path, int font_size) 
{

    IncreaseCacheSize(font_cache_index, &font_cache_size, sizeof(FontCache *), (void *)font_cache);

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

    for (unsigned short int i = 0; i < font_cache_index; i++) {

        if (font_cache[i].font_size == font_size && strcmp(font_cache[i].asset_path, asset_path) == 0) 
            return font_cache[i].font;

    }
    //LogF("Returning new font %s with size %d", asset_path, font_size);

    return GetNewlyAddedFontFromCache(asset_path, font_size);

}