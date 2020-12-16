#include <allegro5/allegro.h>

#include "draw.h"
#include "cache.h"
#include "button.h"

#include "log.h"

void InitButton(DrawObject *object) 
{

    if (object->asset_path != NULL)
        object->button.button_bitmap = GetBitmapFromCache(object->asset_path);

}

void DrawButton(DrawObject *object) 
{

    if (object->bit_flags & BUTTON_MOUSE_HOVERING) {

        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
        DrawGenericTinted(object->button.button_bitmap, object->x, object->y, al_map_rgba(255, 255, 255, 225));
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);

    } else if (object->bit_flags & BUTTON_HAS_TINT) {

        RGBA *tint_color = object->button.tint_color;
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
        DrawGenericTinted(object->button.button_bitmap, object->x, object->y, al_map_rgba(tint_color->r, tint_color->g, tint_color->b, tint_color->a));
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);

    } else {

        DrawGeneric(object->button.button_bitmap, object->x, object->y);

    }

}

void SetButtonTint(DrawObject *object, RGBA *tint_color) 
{

    if (object == NULL || tint_color == NULL)
        return;

    if (object->bit_flags & BUTTON_HAS_TINT)
        free(object->button.tint_color);

    object->button.tint_color = tint_color;
    object->bit_flags |= BUTTON_HAS_TINT;

}

void ClearButtonTint(DrawObject *object)
{

    if (object == NULL)
        return;

    if (object->bit_flags & BUTTON_HAS_TINT)
        free(object->button.tint_color);

    object->button.tint_color = NULL;
    object->bit_flags ^= BUTTON_HAS_TINT;

}

void CleanUpButton(DrawObject *object) 
{

    if (object == NULL)
        return;

    if (object->bit_flags & BUTTON_HAS_TINT)
        free(object->button.tint_color);

    object->button.tint_color = NULL;

}