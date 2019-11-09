#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "drawlayers.h"
#include "shared.h"
#include "log.h"

#include "mainmenu.h"
#include "generalpurposemenus.h"

static int MAX_OBJECTS_PER_LAYER = 0;
static const int MAX_CHARS_IN_TEXTBOX  = 128;

bool IsMouseClickInAreaOfObject(DrawObject *object, int x, int y) 
{

    if (object->x > x || x > object->x + object->width)
        return false;
    if (object->y > y || y > object->y + object->height)
        return false;

    return true;

}

bool HandleMouseClick(DrawObject *object, int x, int y) 
{

    if (IsMouseClickInAreaOfObject(object, x, y)) {

        if (object->button.Callback != NULL) {

            object->button.Callback();
            return true;

        }

    }
    return false;

}

bool HandledMouseClickInButtonAreas(int x, int y) 
{
    
    DrawObject **objects = GetAllDrawObjectsInCurrentLayer();

    if (objects == NULL)
        return false;

    for (int i = 0; i < MAX_OBJECTS_PER_LAYER;i++)
        if (objects[i] != NULL && objects[i]->type == BUTTON)
            if (HandleMouseClick(objects[i], x, y))
                return true;

    return false;

}

bool ToggledTextBoxActiveFlag(DrawObject *object, int x, int y) 
{

    if (IsMouseClickInAreaOfObject(object, x, y)) {

        SetActiveTextBox(object);
        return true;

    }
    return false;

}

bool HandledMouseClickInTextbox(int x, int y)
{

    DrawObject **objects = GetAllDrawObjectsInCurrentLayer();

    if (objects == NULL)
        return false;

    for (int i = 0;i < MAX_OBJECTS_PER_LAYER;i++)
        if (objects[i] != NULL && objects[i]->type == TEXTBOX)
            if(ToggledTextBoxActiveFlag(objects[i], x, y))
                return true;

    return false;

}

void InitializeControls() 
{

    al_install_mouse();
    al_install_keyboard();
    MAX_OBJECTS_PER_LAYER = GetMaxObjectsPerDrawLayer();

}

void HandleMouseInput(ALLEGRO_EVENT event) 
{

    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1) {

        if (HandledMouseClickInButtonAreas(event.mouse.x, event.mouse.y))
            return;
        if (HandledMouseClickInTextbox(event.mouse.x, event.mouse.y))
            return;

    }

}

void HandlePauseMenu(ALLEGRO_EVENT event)
{

    if(event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {

        if(DoesObjectExistInCurrentDrawLayer("LoadingVideo") || DoesObjectExistInCurrentDrawLayer("StartUpVideo"))
            return;
        if(!DoesObjectExistInCurrentDrawLayer("OptionsMenu"))
            TogglePauseMenu();
   
    }

}

void RemoveSingleCharacterFromTextBox(DrawObject *object)
{

    if (object->textbox.current_character < 0)
        return;

    object->textbox.text[object->textbox.current_character] = '\0';
    object->textbox.current_character--;

}

void InsertSingleCharacterIntoTextBox(DrawObject *object, const char *key_pressed, bool to_upper) 
{

    if (object->textbox.current_character >= MAX_CHARS_IN_TEXTBOX)
        return;
    if (strlen(key_pressed) > 1)
        return;

    bool good_char = false;
    if (object->textbox.accept_alphabet_characters && isalpha(*key_pressed))
        good_char = true;
    else if (object->textbox.accept_number_characters && isdigit(*key_pressed))
        good_char = true;

    if (good_char) {

        object->textbox.current_character++;
        object->textbox.text[object->textbox.current_character] = to_upper ? toupper(*key_pressed) : tolower(*key_pressed);

    }


}

bool ShouldUpperCase(ALLEGRO_EVENT event) 
{
    int modifiers   = event.keyboard.modifiers;
    bool shift_caps = ALLEGRO_KEYMOD_SHIFT | ALLEGRO_KEYMOD_CAPSLOCK;
    bool shift      = ALLEGRO_KEYMOD_SHIFT;
    bool caps       = ALLEGRO_KEYMOD_CAPSLOCK;

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

    if (object == NULL || !object->textbox.active)
        return;

    if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        ModifyTextBox(object, event);

}

void HandleKeyboard(ALLEGRO_EVENT event) 
{

    HandlePauseMenu(event);

}

void HandleInput(ALLEGRO_EVENT event) 
{

    HandleMouseInput(event);
    HandleKeyboard(event);
    
}