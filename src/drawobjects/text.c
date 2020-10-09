
#include <math.h>
#include <string.h>

#include <allegro5/allegro.h>

#include "log.h"
#include "draw.h"
#include "cache.h"
#include "drawobject.h"

void InitText(DrawObject *object)
{

    if (object->text.bitmap_path != NULL) {
        object->text.bitmap = GetBitmapFromCache(object->text.bitmap_path);
        object->text.x_offset = 10;
        object->text.y_offset = 10;
    }

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

void DrawWrappedText(DrawObject *object)
{

    float actual_width = object->width - object->text.x_offset;
    float ratio = al_get_text_width(object->text.font, object->text.content)/actual_width;
    if (ratio <= 1.0) {

        al_draw_text(object->text.font, object->text.color, object->x + object->text.x_offset, object->y + object->text.y_offset, 0, object->text.content);
        return;

    }
    size_t max_chars_in_line = (size_t)(strlen(object->text.content)/ratio);
    size_t num_lines = (size_t)ceil(ratio);

    char buffer[max_chars_in_line+1];
    for (size_t i = 0; i < num_lines;i++) {

        strncpy(buffer, object->text.content + (i * max_chars_in_line), max_chars_in_line);
        buffer[max_chars_in_line] = '\0';
        al_draw_text(object->text.font, object->text.color, object->x + object->text.x_offset, object->y + object->text.y_offset + (i * object->text.font_size), 0, buffer);

    }


}

void DrawText(DrawObject *object) 
{

    if (object->text.bitmap_path != NULL)
        DrawGeneric(object->text.bitmap, object->x, object->y);

    DrawWrappedText(object);

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

void SetTextColor(DrawObject *object, unsigned char r, unsigned char g, unsigned char b, unsigned char a) 
{

    if (object == NULL)
        return;

    assert(object->type == TEXT);

    object->text.r = r;
    object->text.g = g;
    object->text.b = b;
    object->text.a = a;

    object->text.color = al_map_rgba(object->text.r, object->text.g, object->text.b, object->text.a);

}

DrawObject *Text_Create()
{

    DrawObject *object       = CreateNewDrawObject();
    object->asset_path       = "assets/font/DanielLinssenM5/m5x7.ttf";
    object->width            = 325;
    object->type             = TEXT;
    object->text.bitmap_path = NULL;
    object->text.bitmap      = NULL;
    object->text.x_offset    = 0;
    object->text.y_offset    = 0;

    object->text.content     = "Placeholder";
    object->text.font_size   = 30;
    object->text.r           = 38;
    object->text.g           = 50;
    object->text.b           = 56;
    object->text.a           = 255;

    return object;

}