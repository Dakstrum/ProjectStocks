
#include <allegro5/allegro.h>

#include "log.h"
#include "draw.h"
#include "cache.h"
#include "drawobject.h"

void InitText(DrawObject *object)
{

    if (object->text.bitmap_path != NULL)
        object->text.bitmap = GetBitmapFromCache(object->text.bitmap_path);

    object->text.font  = GetFontFromCache(object->asset_path, object->text.font_size);
    object->text.color = al_map_rgba(object->text.r, object->text.g, object->text.b, object->text.a);

}

void CleanUpText(DrawObject *object) 
{

    if (object->bit_flags & TEXT_IS_DYNAMIC)
        free(object->text.content);

    object->text.content = NULL;
    object->text.font    = NULL;

}

void DrawText(DrawObject *object) 
{

    if (object->text.bitmap_path != NULL)
        DrawGeneric(object->text.bitmap, object->x, object->y);

    al_draw_text(object->text.font, object->text.color, object->x, object->y, 0, object->text.content);

}

void SetTextContent(DrawObject *object, const char *str, ...) 
{

    if (object == NULL)
        return;

    assert(object->type == TEXT);

    va_list args;
    va_start(args, str);
    
    if (object->bit_flags & TEXT_IS_DYNAMIC) {
        
        SetFormattedPointerVaList(object->text.content, str, args);

    } else { 

        object->bit_flags   |= TEXT_IS_DYNAMIC;
        object->text.content = GetFormattedPointerVaList(str, args);

    }

}

DrawObject *Text_Create()
{

    DrawObject *object       = CreateNewDrawObject();
    object->asset_path       = "assets/font/DanielLinssenM5/m5x7.ttf";
    object->type             = TEXT;
    object->text.bitmap_path = NULL;
    object->text.bitmap      = NULL;

    object->text.content   = "Placeholder";
    object->text.font_size = 30;
    object->text.r         = 38;
    object->text.g         = 50;
    object->text.b         = 56;
    object->text.a         = 255;

    return object;

}