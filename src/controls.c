#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "log.h"
#include "audio.h"
#include "shared.h"
#include "window.h"
#include "mainmenu.h"
#include "drawlayers.h"
#include "generalpurposemenus.h"
#include "account.h"
#include "pausemenus.h"
#include "scrollbox.h"

// leave last character as null character. Leave one character for flicker;
static const int MAX_CHARS_IN_TEXTBOX  = 126;
static WindowScale scale;

bool IsMouseCursorInAreaOfObject(DrawObject *object, int x, int y) 
{

    if (object->x * scale.x_scale > x || x > (object->x + object->width) * scale.x_scale)
        return false;
    if (object->y * scale.y_scale > y || y > (object->y + object->height) * scale.y_scale)
        return false;

    return true;

}

bool HandleMouseClick(DrawObject *object, int x, int y) 
{

    if (IsMouseCursorInAreaOfObject(object, x, y)) {

        if (object->button.Callback != NULL) {

            audio_play_sample(audio_get_sample_id("button_click"));
            object->button.Callback();
            return true;

        }

    }
    return false;

}

bool HandledMouseClickInButtonAreas(int x, int y) 
{

    DrawObjectCollection *collection = GetObjectsByType(BUTTON); 

    if (collection == NULL)
        return false;

    bool handled_mouse_click = false;
    for (int i = 0; i < collection->num_objects;i++) {

        if (HandleMouseClick(collection->objects[i], x, y)) {

            handled_mouse_click = true;
            break;

        }

    }
    DisposeDrawObjectTypeCollection(collection);
    return handled_mouse_click;

}

bool ToggledTextBoxActiveFlag(DrawObject *object, int x, int y) 
{

    if (IsMouseCursorInAreaOfObject(object, x, y)) {

        SetActiveTextBox(object);
        return true;

    }
    return false;

}

bool HandledMouseClickInTextbox(int x, int y)
{

    DrawObjectCollection *collection = GetObjectsByType(TEXTBOX);

    if (collection == NULL)
        return false;

    bool handled_mouse_click = false;
    for (int i = 0;i < collection->num_objects;i++) {

        if(ToggledTextBoxActiveFlag(collection->objects[i], x, y)) {

            handled_mouse_click = true;
            break;

        }

    }
    DisposeDrawObjectTypeCollection(collection);
    return handled_mouse_click;

}

bool MouseInScrollBoxArea(DrawObject *object, const int x, const int y, const int idx)
{

    int box_y = idx * object->scrollbox.vertical_spacing + object->scrollbox.vertical_offset + object->y;

    if (x < object->x * scale.x_scale || x > (object->x + object->scrollbox.box_width) * scale.x_scale)
        return false;
    if (y < box_y * scale.y_scale || y > (box_y + object->scrollbox.box_height) * scale.y_scale)
        return false;

    return true;

}

bool CheckForScrollboxClick(DrawObject *object, const int x, const int y) 
{

    for (int i = 0; i < object->scrollbox.num_items; i++) {

        if (MouseInScrollBoxArea(object, x, y, i)) {

            audio_play_sample(audio_get_sample_id("button_click"));
            ScrollboxText *text = object->scrollbox.text_content[i]->elements;
            object->scrollbox.box_click(text[0].text, i);
            break;

        }

    }

    return false;
}

bool HandledMouseClickInScrollBox(int x, int y)
{

    DrawObjectCollection *collection = GetObjectsByType(SCROLLBOX);
    if (collection == NULL)
        return false;

    for (int i = 0; i < collection->num_objects; i++) {

        if (CheckForScrollboxClick(collection->objects[i], x, y)) {

            DisposeDrawObjectTypeCollection(collection);
            return true;

        }

    }

    return false;
    
}

void InitializeControls() 
{

    al_install_mouse();
    al_install_keyboard();

}

void SetActiveTextBoxToInactive() 
{

    DrawObject *object = GetActiveTextBox();
    if (object != NULL)
        object->bit_flags ^= (object->bit_flags & TEXTBOX_ACTIVE);
        
}

bool HandleScrollEventInArea(DrawObject *object, ALLEGRO_EVENT event)
{

    if (!IsMouseCursorInAreaOfObject(object, event.mouse.x, event.mouse.y))
        return false;

    object->scrollbox.vertical_offset += event.mouse.dz * 20;

    if (object->scrollbox.vertical_offset < object->scrollbox.min_vertical_offset)
        object->scrollbox.vertical_offset = object->scrollbox.min_vertical_offset;
    else if (object->scrollbox.vertical_offset > object->scrollbox.max_vertical_offset)
        object->scrollbox.vertical_offset = object->scrollbox.max_vertical_offset;

    return true;

}

void HandleScrollWheelEvent(ALLEGRO_EVENT event)
{

    DrawObjectCollection *collection = GetObjectsByType(SCROLLBOX);
    if (collection == NULL)
        return;

    for (int i = 0; i < collection->num_objects; i++)
        if (HandleScrollEventInArea(collection->objects[i], event))
            break;

    DisposeDrawObjectTypeCollection(collection);

}

void Control_GraphOverhead(ALLEGRO_EVENT event) 
{

    DrawObjectCollection *collection = GetObjectsByType(GRAPH);
    if (collection == NULL)
        return;

    for (int i = 0;i < collection->num_objects; i++) {

        if (IsMouseCursorInAreaOfObject(collection->objects[i], event.mouse.x, event.mouse.y)) {

            collection->objects[i]->graph.m_x = event.mouse.x / scale.x_scale;
            collection->objects[i]->graph.m_y = event.mouse.y / scale.y_scale;

        } else {

            collection->objects[i]->graph.m_x = -1.0;
            collection->objects[i]->graph.m_y = -1.0;

        }

    }

    DisposeDrawObjectTypeCollection(collection);

}

void HandleMouseInput(ALLEGRO_EVENT event) 
{

    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1) {

        if (HandledMouseClickInButtonAreas(event.mouse.x, event.mouse.y)) {

            SetActiveTextBoxToInactive();
            return;

        }
        if (HandledMouseClickInTextbox(event.mouse.x, event.mouse.y))
            return;
        else if (HandledMouseClickInScrollBox(event.mouse.x, event.mouse.y))
            return;

        SetActiveTextBoxToInactive();

    } else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {

        if (event.mouse.dz != 0)
            HandleScrollWheelEvent(event);

    }


}

void HandlePauseMenu(ALLEGRO_EVENT event)
{

    if (event.type != ALLEGRO_EVENT_KEY_CHAR || event.keyboard.keycode != ALLEGRO_KEY_ESCAPE)
        return;

    if(DoesObjectExistInCurrentDrawLayer("LoadingVideo") || DoesObjectExistInCurrentDrawLayer("StartUpVideo") || DoesObjectExistInCurrentDrawLayer("DisplayMenu"))
        return;

    if(Account_GetInGameStatus() == 1)
    {
        if(!DoesObjectExistInCurrentDrawLayer("OptionsMenu"))
            PauseMenu_Toggle();
    }

    else
        Log("Toggle OutOfGamePauseMenu if needed in the future");

}

void RemoveSingleCharacterFromTextBox(DrawObject *object)
{

    if (object->textbox.current_character < 0)
        return;

    object->textbox.text[object->textbox.current_character] = '\0';
    object->textbox.current_character--;

}

bool IsItAGoodCharacter(DrawObject *object, const char *key_pressed)
{

    bool good_char = false;
    if (object->bit_flags & TEXTBOX_ACCEPT_ALPHABET_CHARACTERS && (isalpha(*key_pressed) || *key_pressed == ' '))
        good_char = true;
    else if (object->bit_flags & TEXTBOX_ACCEPT_NUMBER_CHARACTERS && isdigit(*key_pressed))
        good_char = true;

    return good_char;

}

void InsertSingleCharacterIntoTextBox(DrawObject *object, const char *key_pressed, bool to_upper) 
{

    if (object->textbox.current_character >= MAX_CHARS_IN_TEXTBOX)
        return;
    if (object->textbox.current_character+1 == object->textbox.limit_characters_to)
        return;
    if (strlen(key_pressed) > 1)
        return;

    if (IsItAGoodCharacter(object, key_pressed)) {

        object->textbox.current_character++;
        object->textbox.text[object->textbox.current_character] = to_upper ? toupper(*key_pressed) : tolower(*key_pressed);

    }


}

bool ShouldUpperCase(ALLEGRO_EVENT event) 
{
    int modifiers                = event.keyboard.modifiers;
    static const int shift_caps  = ALLEGRO_KEYMOD_SHIFT | ALLEGRO_KEYMOD_CAPSLOCK;
    static const int shift       = ALLEGRO_KEYMOD_SHIFT;
    static const int caps        = ALLEGRO_KEYMOD_CAPSLOCK;

    if ((modifiers & shift_caps) == shift_caps)
        return false;
    else if (modifiers & shift)
        return true;
    else if (modifiers & caps)
        return true;

    return false;

}

void ModifyTextBox(DrawObject *object, ALLEGRO_EVENT event)
{

    bool to_upper = ShouldUpperCase(event);
    switch (event.keyboard.keycode) {

        case ALLEGRO_KEY_BACKSPACE:
        case ALLEGRO_KEY_DELETE:  RemoveSingleCharacterFromTextBox(object); break;
        case ALLEGRO_KEY_SPACE:   InsertSingleCharacterIntoTextBox(object, " ", false); break;
        default: InsertSingleCharacterIntoTextBox(object, al_keycode_to_name(event.keyboard.keycode), to_upper); break;

    }

}

void HandleTextBoxInput(ALLEGRO_EVENT event)
{

    DrawObject *object = GetActiveTextBox();

    if (object == NULL || !(object->bit_flags & TEXTBOX_ACTIVE))
        return;

    if (event.type == ALLEGRO_EVENT_KEY_CHAR)
        ModifyTextBox(object, event);

}

void HandleKeyboard(ALLEGRO_EVENT event) 
{

    HandlePauseMenu(event);
    HandleTextBoxInput(event);

}

void HandleInput(ALLEGRO_EVENT event) 
{

    scale = GetWindowScale();
    HandleMouseInput(event);
    HandleKeyboard(event);
    Control_GraphOverhead(event);
    
}

void TintButtons()
{

    static ALLEGRO_MOUSE_STATE state;
    al_get_mouse_state(&state);

    DrawObjectCollection *collection = GetObjectsByType(BUTTON);

    if (collection == NULL)
        return;

    for (int i = 0;i < collection->num_objects;i++) {

        if (IsMouseCursorInAreaOfObject(collection->objects[i], state.x, state.y))
            collection->objects[i]->bit_flags |= BUTTON_MOUSE_HOVERING;
        else if (collection->objects[i]->bit_flags & BUTTON_MOUSE_HOVERING)
            collection->objects[i]->bit_flags ^= BUTTON_MOUSE_HOVERING;   

    }
    DisposeDrawObjectTypeCollection(collection);

}

int HoveringOverBitmap(DrawObject *bitmap)
{

    static ALLEGRO_MOUSE_STATE state;
    al_get_mouse_state(&state);

    if (IsMouseCursorInAreaOfObject(bitmap, state.x, state.y)) {

        bitmap->bit_flags |= BUTTON_MOUSE_HOVERING;
        return 1;

    } else if (bitmap->bit_flags & BUTTON_MOUSE_HOVERING) {

        bitmap->bit_flags ^= BUTTON_MOUSE_HOVERING;  
        return 0;

    }
    return 0;

}

void TintScrollBox()
{

    static ALLEGRO_MOUSE_STATE state;
    al_get_mouse_state(&state);

    DrawObjectCollection *collection = GetObjectsByType(SCROLLBOX);

    if (collection == NULL)
        return;

    DrawObject *object = NULL;
    for (int i = 0;i < collection->num_objects;i++) {

        object = collection->objects[i];
        object->scrollbox.currently_tinted = -1;
        for (int k = 0;k < object->scrollbox.num_items;k++) {

            if (MouseInScrollBoxArea(object, state.x, state.y, k)) {

                object->scrollbox.currently_tinted = k;
                break;

            }

        }

    }
    DisposeDrawObjectTypeCollection(collection);

}

void HandleMouseLocation()
{

    TintButtons();
    TintScrollBox();

}