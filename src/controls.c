#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "drawlayers.h"
#include "shared.h"
#include "dbaccess.h"

void InitializeControls();
void HandleMouseClicks();

static ALLEGRO_EVENT_QUEUE *event_queue;
static ALLEGRO_TIMER *timer;

void StartInputLoop() 
{

    InitializeControls();
    bool should_clean_up;
    while (!(should_clean_up = ShouldICleanUp())) {

        char s[50];
        sprintf(s, "%d", should_clean_up);
        Log(s);
        HandleMouseClicks();

    }
    Log("Quiting Input Loop");

}

void InitializeControls() 
{

    al_install_mouse();
    event_queue = al_create_event_queue();
    timer       = al_create_timer(1.0/144.0);

    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

}

void HandleMouseClicks() 
{

    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);
    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1) {

        HandleMouseClickInButtonAreas(event.mouse.x, event.mouse.y);

    }

}
