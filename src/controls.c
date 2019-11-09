#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "drawlayers.h"
#include "shared.h"
#include "log.h"

#include "mainmenu.h"
#include "generalpurposemenus.h"

static int MAX_OBJECTS_PER_LAYER = 0;

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

        SetAllTextBoxesToInactiveInCurrentDrawLayer();
        object->textbox.active = true;
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

void HandleKeyboard(ALLEGRO_EVENT event) 
{

    HandlePauseMenu(event);

}

void HandleInput(ALLEGRO_EVENT event) 
{

    HandleMouseInput(event);
    HandleKeyboard(event);
    
}