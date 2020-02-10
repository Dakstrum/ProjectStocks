#include <allegro5/allegro.h>

#include "draw.h"
#include "cache.h"
#include "button.h"

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

    } else {

        DrawGeneric(object->button.button_bitmap, object->x, object->y);

    }

}

void CleanUpButton(DrawObject *object) 
{


}