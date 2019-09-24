#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "drawlayers.h"
#include "shared.h"
#include "log.h"

void HandleMouseClicks(ALLEGRO_EVENT event);
void HandleKeyboard(ALLEGRO_EVENT event);
void HandlePauseMenu(ALLEGRO_EVENT event);

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
    HandlePauseMenu(event);

}

void HandleMouseClicks(ALLEGRO_EVENT event) 
{

    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1)
        HandleMouseClickInButtonAreas(event.mouse.x, event.mouse.y);

}

void HandleKeyboard(ALLEGRO_EVENT event) 
{


}

void HandlePauseMenu(ALLEGRO_EVENT local_event)
{
    
    ALLEGRO_EVENT_QUEUE *event_queue;
    //ALLEGRO_EVENT event;
    //al_wait_for_event(event_queue, &event);

    //if (event.type == ALLEGRO_EVENT_KEY_DOWN)
    //{
    //    Log("SOEMTHING OPERESS");
    //    if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
    //        Log("ASD");
    //}
    

}
