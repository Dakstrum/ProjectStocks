
#include <math.h>
#include <string.h>
#include <locale.h>

#include <allegro5/allegro.h>

#include "log.h"
#include "draw.h"
#include "cache.h"
#include "vector.h"
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
    float object_width = object->width - object->text.x_offset;

    float used_width   = 0;
    float space_width  = al_get_text_width(object->text.font, " ");
    int line           = 0;
    char *str          = strdup(object->text.content);
    char *word         = strtok(str, " ");
    while (word != NULL) {

        float word_width = al_get_text_width(object->text.font, word);
        if (word_width + space_width + used_width + object->text.x_offset > object_width) {

            used_width = 0;
            line++;

        }
        al_draw_text(object->text.font, object->text.color, object->x + object->text.x_offset + used_width, object->y + object->text.y_offset + (line * object->text.font_size), 0, word);
        used_width += word_width + space_width;
        word = strtok(NULL, " ");

    }
    free(str);

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

void SetTextContentWithCommaFormat(DrawObject *object, const char *str, ...) 
{

    char *oldLocale = setlocale(LC_NUMERIC, NULL);
    setlocale(LC_NUMERIC, "");
    
    va_list args;
    va_start(args, str);
    char *buffer = GetFormattedPointerVaList(str, args);
    va_end(args);

    SetTextContent(object, buffer);

    setlocale(LC_NUMERIC, oldLocale);
    free(buffer);

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
    object->width            = 400;
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