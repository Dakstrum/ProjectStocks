#include <stdbool.h>

#include <allegro5/allegro.h>

#include "drawlayers.h"
#include "shared.h"

void InitializeControls();
void HandleMouseClicks();

static ALLEGRO_EVENT_QUEUE *event_queue;
void StartInputLoop() 
{

    InitializeControls();
    while (!ShouldICleanUp()) {

        HandleMouseClicks();

    }

}

void InitializeControls() 
{

    al_install_mouse();
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_mouse_event_source());

}

void HandleMouseClicks() 
{

    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);
    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1) {

        HandleMouseClickInButtonAreas(event.mouse.x, event.mouse.y);

    }

}
