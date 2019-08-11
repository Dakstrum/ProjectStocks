#include <stdio.h>

#include <allegro5/allegro.h>

#include "shared.h"
#include "dbaccess.h"

void InitializeDisplay();
void RenderingLoop();
void RenderFrame();
void CleanUpDisplay();
void HandleWindowEvents();

static float FPS = 60.0; 

static ALLEGRO_DISPLAY *display         = NULL;
static ALLEGRO_TIMER *timer             = NULL;
static ALLEGRO_EVENT_QUEUE *event_queue = NULL;
void *RenderingEntry(ALLEGRO_THREAD *thread, void *arg) 
{

    InitializeDisplay();
    RenderingLoop();
    CleanUpDisplay();

    Log("Quiting Rendering Thread");

    return NULL;

}

void InitializeDisplay() 
{

    display     = al_create_display(1920, 1080);
    event_queue = al_create_event_queue();
    timer       = al_create_timer(1.0/FPS);
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));


}

void RenderingLoop() 
{

    while (!ShouldICleanUp()) {

        RenderFrame();
        HandleWindowEvents();

    }

}

void RenderFrame() 
{


}

void HandleWindowEvents() 
{

    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);
    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {

        SetCleanUpToTrue();

    }

}

void CleanUpDisplay() 
{

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

}