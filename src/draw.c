
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "shared.h"

#include "log.h"
#include <allegro5/allegro.h>                                       
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include<stdio.h> 

static WindowScale scale;

ALLEGRO_FONT *font = NULL;

void RefreshDrawScale()
{

    scale = GetWindowScale();

}

void DrawGeneric(ALLEGRO_BITMAP *bitmap, float x, float y) 
{

    if (bitmap == NULL)
        return;

    const float scale_width  = al_get_bitmap_width(bitmap);
    const float scale_height = al_get_bitmap_height(bitmap);
    al_draw_scaled_bitmap(bitmap, 0, 0, scale_width, scale_height, x, y, scale_width, scale_height, 0);

}

void DrawGenericTinted(ALLEGRO_BITMAP *bitmap, float x, float y, ALLEGRO_COLOR tint)
{

    if (bitmap == NULL)
        return;

    const float scale_width  = al_get_bitmap_width(bitmap);
    const float scale_height = al_get_bitmap_height(bitmap);
    al_draw_tinted_scaled_bitmap(bitmap, tint, 0, 0, scale_width, scale_height, x, y, scale_width, scale_height, 0);

}

void DrawGenericWithWidth(ALLEGRO_BITMAP *bitmap, float x, float y, float width, float height) 
{

    if (bitmap == NULL)
        return;

    const float scale_width  = al_get_bitmap_width(bitmap);
    const float scale_height = al_get_bitmap_height(bitmap);
    al_draw_scaled_bitmap(bitmap, 0, 0, scale_width, scale_height, x, y, width, height, 0);

}

void DrawBackBuffer(ALLEGRO_BITMAP *bitmap) 
{

    if (bitmap == NULL)
        return;

    const float scale_width  = (float)al_get_bitmap_width(bitmap)  * scale.x_scale;
    const float scale_height = (float)al_get_bitmap_height(bitmap) * scale.y_scale;
    al_draw_scaled_bitmap(bitmap, 0, 0, 1920, 1080, 0, 0, scale_width, scale_height, 0);

}

void DrawFrameTiming()
{
    static char FPS_string[255];
    if(font == NULL)
        font = al_load_ttf_font("assets/font/DanielLinssenM5/m5x7.ttf", 50, 0);



    sprintf(FPS_string, "%d", 60); 
    al_draw_text(font , al_map_rgb(0, 0, 0), 0, 0, 0, FPS_string);

}void DrawFrameTiming()
{
    static char FPS_string[255];
    if(font == NULL)
        font = al_load_ttf_font("assets/font/DanielLinssenM5/m5x7.ttf", 50, 0);

    


    sprintf(FPS_string, "%d", i); 
    al_draw_text(font , al_map_rgb(0, 0, 0), 0, 0, 0, FPS_string);

}