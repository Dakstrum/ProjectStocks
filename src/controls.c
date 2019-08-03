#include <allegro5/allegro.h>

#include "drawlayers.h"
#include "shared.h"

bool PlayerWantsToQuitGame();
void HandleMouseClicks();


void StartInputLoop() 
{

    while (1) {

        HandleMouseClicks();
        if(PlayerWantsToQuitGame())
            break;

    }

}

static ALLEGRO_EVENT event;
void HandleMouseClicks() 
{

    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1) {

        HandleMouseClickInButtonAreas(event.mouse.x, event.mouse.y);

    }

}

bool PlayerWantsToQuitGame() 
{

    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {

        return true;

    }
    return false;

}