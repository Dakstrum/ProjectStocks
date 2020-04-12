#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "log.h"
#include "vector.h"

typedef struct BitmapCache {

    char *asset_path;
    ALLEGRO_BITMAP *bitmap;

} BitmapCache;

typedef struct FontCache {

    char *asset_path;
    ALLEGRO_FONT *font;
    int font_size;

} FontCache;

static Vector *bitmap_vector = NULL;
static Vector *font_vector   = NULL;

void InitializeCache() 
{

    bitmap_vector = CreateVector(sizeof(BitmapCache), 128);
    font_vector   = CreateVector(sizeof(FontCache), 128);

}

ALLEGRO_BITMAP *GetNewlyAddedBitmapFromCache(char *asset_path) 
{

    BitmapCache cache = {asset_path, al_load_bitmap(asset_path)};
    PushBack(bitmap_vector, &cache);
    return cache.bitmap;
    
}


ALLEGRO_BITMAP *GetBitmapFromCache(char *asset_path) 
{

    if (asset_path == NULL || strcmp(asset_path,"") == 0)
        return NULL;

    BitmapCache *cache = (BitmapCache *)bitmap_vector->elements;

    for (unsigned short int i = 0; i < bitmap_vector->num_elements; i++)
        if (strcmp(asset_path, cache[i].asset_path) == 0)
            return cache[i].bitmap;

    return GetNewlyAddedBitmapFromCache(asset_path);

}

ALLEGRO_FONT *GetNewlyAddedFontFromCache(char *asset_path, int font_size) 
{

    FontCache cache = {asset_path, al_load_ttf_font(asset_path, font_size, 0), font_size};
    PushBack(font_vector, &cache);
    return cache.font;

}

ALLEGRO_FONT *GetFontFromCache(char *asset_path, int font_size) 
{

    if (asset_path == NULL || font_size == 0)
        return NULL;

    FontCache *cache = (FontCache *)font_vector->elements;

    for (unsigned short int i = 0; i < font_vector->num_elements; i++)
        if (cache[i].font_size == font_size && strcmp(cache[i].asset_path, asset_path) == 0) 
            return cache[i].font;

    return GetNewlyAddedFontFromCache(asset_path, font_size);

}