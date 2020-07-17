
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "window.h"

static WindowScale scale;

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

void DrawFrameTime()
{

    
    
}