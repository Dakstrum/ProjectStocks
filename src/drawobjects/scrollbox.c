
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include <allegro5/allegro.h>

#include "log.h"
#include "draw.h"
#include "cache.h"
#include "vector.h"

#include "drawobject.h"
#include "scrollbox.h"

void InitIconPaths(DrawObject *object) 
{

    if (object->scrollbox.icons == NULL)
        return;

    for (size_t i = 0; i < object->scrollbox.num_items;i++) {

        ScrollboxIcon *icons = object->scrollbox.icons[i]->elements;
        for (size_t k = 0; k < object->scrollbox.icons[i]->num_elements;k++) {

            icons[k].icon = GetBitmapFromCache(icons[k].icon_path);

        }

    }

}

void InitTextFont(DrawObject *object)
{

    if (object->scrollbox.text_content == NULL)
        return;

    for (size_t i = 0; i < object->scrollbox.num_items;i++) {

        ScrollboxText *text = object->scrollbox.text_content[i]->elements;
        for (size_t k = 0; k < object->scrollbox.text_content[i]->num_elements;k++) {

            text[k].font = GetFontFromCache(object->scrollbox.text_style->font_path, text[k].font_size);

        }

    }

}

void Scrollbox_Init(DrawObject *object)
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

    InitIconPaths(object);
    InitTextFont(object);

    assert(object->scrollbox.text_style->font != NULL);

    if (object->scrollbox.text_style->font == NULL)
        Log("font is null");

}

void Scrollbox_DrawText(DrawObject *object, int i, int x, int box_y)
{

    size_t num_elements = object->scrollbox.text_content[i]->num_elements;
    ScrollboxText *text = object->scrollbox.text_content[i]->elements;

    for (size_t k = 0; k < num_elements;k++)
        al_draw_text(text[k].font, object->scrollbox.text_style->color, x + text[k].rx, box_y + text[k].ry, 0, text[k].text);

}

void Scrollbox_DrawIcons(DrawObject *object, int i, int x, int box_y)
{

    size_t num_elements = object->scrollbox.icons[i]->num_elements;
    ScrollboxIcon *icons = object->scrollbox.icons[i]->elements;

    for (size_t k = 0; k < num_elements;k++)
        DrawGeneric(icons[k].icon, x + icons[k].rx, box_y + icons[k].ry);

}

void DrawRegularBox(DrawObject *object, int i, int x, int box_y)
{

    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    DrawGenericTinted(object->scrollbox.boxes_bitmap, x, box_y, al_map_rgba(255, 255, 255, object->scrollbox.currently_tinted == i ? 150 : 255));

    if (object->scrollbox.icons != NULL)
        Scrollbox_DrawIcons(object, i, x, box_y);

    if (object->scrollbox.text_content != NULL)
        Scrollbox_DrawText(object, i, x, box_y);


    al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);

}

void DrawOffsettedBox(DrawObject *object, int i, int x_offsetted, int box_y, unsigned char alpha_mask)
{

    DrawGenericTinted(object->scrollbox.boxes_bitmap, x_offsetted, box_y, al_map_rgba(255, 255, 255, alpha_mask));

    if (object->scrollbox.icons != NULL)
        Scrollbox_DrawIcons(object, i, x_offsetted, box_y);

    if (object->scrollbox.text_content != NULL)
        Scrollbox_DrawText(object, i, x_offsetted, box_y);


}

void Scrollbox_Draw(DrawObject *object) 
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

            DrawOffsettedBox(object, i, x + x_offset, box_y, alpha_mask);

        } else {

            DrawRegularBox(object, i, x, box_y);

        }

    }

}

void CleanUpVectors(DrawObject *object)
{

    if (object->scrollbox.text_content == NULL)
        return;

    for (size_t i = 0; i < object->scrollbox.num_items;i++)
        Vector_Delete(object->scrollbox.text_content[i]);


    if (object->scrollbox.icons == NULL)
        return;

    for (size_t i = 0; i < object->scrollbox.num_items;i++)
        Vector_Delete(object->scrollbox.icons[i]);

}

void Scrollbox_Clean(DrawObject *object)
{
    
    if (object->scrollbox.text_style != NULL)
        free(object->scrollbox.text_style);

    CleanUpVectors(object);

    object->scrollbox.text_content = NULL;
    object->scrollbox.icons        = NULL;
    object->scrollbox.text_style   = NULL;
    
}

DrawObject *Scrollbox_Create()
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
    object->scrollbox.boxes_bitmap          = NULL;

    object->scrollbox.text_content          = NULL;
    object->scrollbox.icons                 = NULL;

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

ScrollboxText Scrollbox_CreateText()
{

    ScrollboxText text;
    text.font = NULL;
    text.text = NULL;

    text.rx = 0;
    text.ry = 0;

    text.font_size = 0;

    return text;

}

ScrollboxIcon Scrollbox_CreateIcon()
{

    ScrollboxIcon icon;

    icon.rx = 0;
    icon.ry = 0;

    icon.icon_path = NULL;
    icon.icon = NULL;

    return icon;

}