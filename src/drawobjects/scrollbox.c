
#include <math.h>
#include <assert.h>

#include <allegro5/allegro.h>

#include "log.h"
#include "draw.h"
#include "cache.h"

#include "drawobject.h"

void InitScrollbox(DrawObject *object)
{
    
    object->scrollbox.boxes_bitmap = GetBitmapFromCache(object->asset_path);
    object->scrollbox.box_width    = al_get_bitmap_width(object->scrollbox.boxes_bitmap);
    object->scrollbox.box_height   = al_get_bitmap_height(object->scrollbox.boxes_bitmap);

    ScrollBox *scrollbox           = &object->scrollbox;
    TextStyle *text_style          = scrollbox->text_style;
    text_style->font               = GetFontFromCache(text_style->font_path, text_style->font_size);
    text_style->color              = al_map_rgba(text_style->r, text_style->g, text_style->b, text_style->a);

    if (object->scrollbox.box_height * object->scrollbox.num_items > object->height) {

        int diff          = object->scrollbox.box_height * object->scrollbox.num_items - object->height;
        int boxes_in_diff = (int)ceil((double)diff / (object->scrollbox.box_height));
        scrollbox->min_vertical_offset = -boxes_in_diff * scrollbox->vertical_spacing;
        scrollbox->max_vertical_offset = 0;

    }

    if (object->scrollbox.text_style->font == NULL)
        Log("font is null");

}

void DrawScrollBox(DrawObject *object) 
{

    const int x = object->x;
    const int vertical_spacing = object->scrollbox.vertical_spacing;
    const int vertical_offset  = object->scrollbox.vertical_offset;

    int box_y = 0;
    for (int i = 0; i < object->scrollbox.num_items; i++) {

        box_y = i * vertical_spacing + vertical_offset + object->y;

        if (box_y < object->y || box_y + (vertical_spacing/2) > object->y + object->height)
            continue;

        float box_with_vertical  = box_y + vertical_spacing;
        float object_with_height = object->y + object->height;
        float crossing_diff      = box_with_vertical - object_with_height;
        if (crossing_diff > 0) {

            float percent_diff       = (crossing_diff / ((box_with_vertical + object_with_height) / 2));
            unsigned char alpha_mask = 255 - (unsigned char)(percent_diff * 255.0 * 4.0);
            float x_offset           = percent_diff * 400.0;

            DrawGenericTinted(object->scrollbox.boxes_bitmap, x + x_offset, box_y, al_map_rgba(255, 255, 255, alpha_mask));
            al_draw_text(object->scrollbox.text_style->font, object->scrollbox.text_style->color, x + 30 + x_offset, box_y + 20, 0, object->scrollbox.text_content[i]);

        } else {

            al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
            DrawGenericTinted(object->scrollbox.boxes_bitmap, x, box_y, al_map_rgba(255, 255, 255, object->scrollbox.currently_tinted == i ? 150 : 255));
            al_draw_text(object->scrollbox.text_style->font, object->scrollbox.text_style->color, x + 30, box_y + 20, 0, object->scrollbox.text_content[i]);
            al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);

        }

    }

}

void CleanUpScrollbox(DrawObject *object)
{

    if (object->scrollbox.icons != NULL)
        free(object->scrollbox.icons);
    
    if (object->scrollbox.text_style != NULL)
        free(object->scrollbox.text_style);

    for (unsigned short int i = 0; i < object->scrollbox.num_items; i++) {

        free(object->scrollbox.text_content[i]);
        object->scrollbox.text_content[i] = NULL;

    }

    if (object->scrollbox.text_content != NULL)
        free(object->scrollbox.text_content);

    if (object->scrollbox.icon_paths != NULL)
        free(object->scrollbox.icon_paths);

    object->scrollbox.icons        = NULL;
    object->scrollbox.text_style   = NULL;
    object->scrollbox.text_content = NULL;
    object->scrollbox.icon_paths   = NULL;
    
}

DrawObject *CreateScrollBoxObject()
{

    DrawObject *object = CreateNewDrawObject();

    object->x                               = 0;
    object->y                               = 0;
    object->width                           = 0;
    object->height                          = 0;
    object->asset_path                      = NULL;
    object->type                            = SCROLLBOX;

    object->scrollbox.num_items             = 0;
    object->scrollbox.currently_tinted      = -1;
    object->scrollbox.box_click             = NULL;
    object->scrollbox.vertical_spacing      = 85;
    object->scrollbox.vertical_offset       = 0;
    object->scrollbox.min_vertical_offset   = 0;
    object->scrollbox.max_vertical_offset   = 0;
    object->scrollbox.box_click             = NULL;
    object->scrollbox.text_content          = NULL;
    object->scrollbox.boxes_bitmap          = NULL;
    object->scrollbox.icons                 = NULL;
    object->scrollbox.icon_paths            = NULL;

    object->scrollbox.text_style            = malloc(sizeof(TextStyle));
    object->scrollbox.text_style->font_size = 40;
    object->scrollbox.text_style->a         = 255;
    object->scrollbox.text_style->r         = 0;
    object->scrollbox.text_style->g         = 0;
    object->scrollbox.text_style->b         = 0;
    object->scrollbox.text_style->font_path = "assets/font/DanielLinssenM5/m5x7.ttf";
    object->scrollbox.text_content          = NULL;

    return object;
}