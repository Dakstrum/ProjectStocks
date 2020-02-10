
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "draw.h"
#include "cache.h"
#include "drawobject.h"

void InitTextBox(DrawObject *object)
{

    if (object->asset_path != NULL)
        object->textbox.bitmap = GetBitmapFromCache(object->asset_path);

    memset(object->textbox.text, '\0', 128);

    object->textbox.text_style->font  = GetFontFromCache(object->textbox.text_style->font_path, object->textbox.text_style->font_size);
    object->textbox.text_style->color = al_map_rgba(object->textbox.text_style->r, object->textbox.text_style->g, object->textbox.text_style->b, object->textbox.text_style->a);

    object->textbox.placeholder_style->font  = GetFontFromCache(object->textbox.placeholder_style->font_path, object->textbox.placeholder_style->font_size);
    object->textbox.placeholder_style->color = al_map_rgba(object->textbox.placeholder_style->r, object->textbox.placeholder_style->g, object->textbox.placeholder_style->b, object->textbox.placeholder_style->a); 

}

void SetModifiedTextBoxWithFlicker(DrawObject *object)
{

    struct timespec current_time = GetCurrentTime();
    if (object->bit_flags & TEXTBOX_FLICKER_DRAWING) {
 
        if (GetMillDiff(&object->textbox.flicker, &current_time) >= 500) {

            object->bit_flags      ^= (object->bit_flags & TEXTBOX_FLICKER_DRAWING);
            object->textbox.flicker = GetCurrentTime();

        } else {

            object->textbox.text[object->textbox.current_character + 1] = '|';

        }

    } else {

        if (GetMillDiff(&object->textbox.flicker, &current_time) >= 250) {

            object->bit_flags       |= TEXTBOX_FLICKER_DRAWING;
            object->textbox.flicker  = GetCurrentTime();

        }

    }

}

void DrawTextBox(DrawObject *object)
{

    DrawGeneric(object->textbox.bitmap, object->x, object->y);
    if (object->textbox.current_character == -1 && !(object->bit_flags & TEXTBOX_ACTIVE)) {

        al_draw_text(object->textbox.placeholder_style->font, object->textbox.placeholder_style->color, object->x, object->y, 0, object->textbox.placeholder_text);

    } else {

        if (object->bit_flags & TEXTBOX_ACTIVE)
            SetModifiedTextBoxWithFlicker(object);

        al_draw_text(object->textbox.text_style->font, object->textbox.text_style->color, object->x, object->y, 0, object->textbox.text);
        object->textbox.text[object->textbox.current_character + 1] = '\0';

    }

}

void CleanUpTextBox(DrawObject *object)
{

}