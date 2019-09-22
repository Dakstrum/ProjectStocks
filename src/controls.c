#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "drawlayers.h"
#include "shared.h"
#include "log.h"

void HandleMouseClicks(ALLEGRO_EVENT event);
void HandleKeyboard(ALLEGRO_EVENT event);

ALLEGRO_KEYBOARD_STATE key_state;

void InitializeControls() 
{

    al_install_mouse();

    al_install_keyboard();

}

void HandleInput(ALLEGRO_EVENT event) 
{

    HandleMouseClicks(event);
    HandleKeyboard(event);

}

void HandleMouseClicks(ALLEGRO_EVENT event) 
{

    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1)
        HandleMouseClickInButtonAreas(event.mouse.x, event.mouse.y);

}

void HandleKeyboard(ALLEGRO_EVENT event) 
{


}

void HandlePauseMenu(ALLEGRO_EVENT event)
{

    al_get_keyboard_state(&key_state);
    if (al_key_down(&key_state, ALLEGRO_KEY_ESCAPE)) {

        Log("Init Pause Menu");
    }
}
