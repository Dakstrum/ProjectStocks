#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "drawlayers.h"
#include "shared.h"
#include "log.h"

void HandleMouseClicks(ALLEGRO_EVENT event);
void HandleKeyboard(ALLEGRO_EVENT event);

void InitializeControls() 
{

    al_install_mouse();

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
