#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "drawlayers.h"
#include "shared.h"
#include "log.h"

#include "mainmenu.h"

void HandleMouseClicks(ALLEGRO_EVENT event);
void HandleKeyboard(ALLEGRO_EVENT event);
void HandlePauseMenu(ALLEGRO_EVENT local_event, ALLEGRO_EVENT_QUEUE *local_event_queue);

void InitializeControls() 
{

    al_install_mouse();
    al_install_keyboard();

}

void HandleInput(ALLEGRO_EVENT event, ALLEGRO_EVENT_QUEUE *local_queue) 
{

    HandleMouseClicks(event);
    HandleKeyboard(event);
    HandlePauseMenu(event, local_queue);

}

void HandleMouseClicks(ALLEGRO_EVENT event) 
{

    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1)
        HandleMouseClickInButtonAreas(event.mouse.x, event.mouse.y);

}

void HandleKeyboard(ALLEGRO_EVENT event) 
{


}

void HandlePauseMenu(ALLEGRO_EVENT local_event, ALLEGRO_EVENT_QUEUE *local_event_queue)
{

    if(local_event.type == ALLEGRO_EVENT_KEY_DOWN && local_event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
        
        TogglePauseMenu();
    }
}
