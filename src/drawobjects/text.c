
#include <allegro5/allegro.h>

#include "draw.h"
#include "cache.h"
#include "drawobject.h"

void InitText(DrawObject *object)
{

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