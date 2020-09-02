#include <stdio.h>
#include <time.h>

#ifdef __linux__
    #include <signal.h>
#endif
#include <allegro5/allegro.h>

#include "dbaccess.h"
#include "dbevents.h"
#include "dbcompany.h"

#include "log.h"
#include "cache.h"
#include "graph.h"
#include "shared.h"
#include "controls.h"
#include "simulation.h"
#include "rendering.h"
#include "audio.h"
#include "jsonlayer.h"
#include "account.h"
#include "buttons.h"
#include "vector.h"
#include "window.h"
#include "animations.h"
#include "drawlayers.h"
#include "drawobject.h"
#include "rendering.h"
#include "draw.h"
#include "timer.h"

enum InitializeSuccess 
{

    FAILURE = 0,
    SUCCESS

};
/* Test code */
static ALLEGRO_EVENT_QUEUE *event_queue;
static ALLEGRO_TIMER *timer;
static Vector *input_events = NULL;

enum InitializeSuccess Initialize();
void GameLoop();
void InitializeGameThreads();
void InitializeEventQueue();
ALLEGRO_EVENT WaitForEvent();
void CleanUp();
void CleanUpThreads();
void StartInputLoop();

int main(int argc, char **argv) 
{

    if (Initialize())
        GameLoop();

    CleanUp();

#ifdef __linux__
    raise(SIGTERM);
#endif
    return 0;

}


void ApplyInputs()
{

    ALLEGRO_EVENT *events = input_events->elements;
    for (size_t i = 0; i < input_events->num_elements;i++) {

        HandleInput(events[i]);
        HandleWindowEvents(events[i]); 

    }
    Vector_Reset(input_events); 

}

void Loop() 
{

    ALLEGRO_EVENT event = WaitForEvent();

    if (event.type == ALLEGRO_EVENT_TIMER) {

        double dt = Timer_GetDiff();
        ApplyInputs();
        Animate_Update(dt);
        HandleMouseLocation();
        HandleRendering(dt);


    } else {

        Vector_PushBack(input_events, &event);

    }

}

void GameLoop() 
{

    input_events = Vector_Create(sizeof(ALLEGRO_EVENT), 32);
    Timer_Init();

    while (!ShouldICleanUp())
        Loop();

}

ALLEGRO_EVENT WaitForEvent() 
{

    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);
    return event;

}

enum InitializeSuccess Initialize() 
{

#if DEBUGGING

    al_set_config_value(al_get_system_config(), "trace", "level", "debug");

#endif

    if (!al_init()) {

        fprintf(stderr, "failed to initialize allegro");
        return FAILURE;

    }

    InitializeLogging();
    InitializeButtons();
    InitializeCache();
    InitializeShared();
    InitializeDatabases();
    InitializeCompanies();
    InitializeEvents();
    InitializeControls();

    InitializeRendering();
    Window_Initialize();
    DrawLayer_Initialize();

    InitializeEventQueue();
    InitializeJson();
    InitializeAudio();
    InitAccount();
    InitializeSimulation();

    DrawObjects_Initialize();
    Animate_Initialize();

    Draw_Initialize();

    return SUCCESS;

}

void InitializeEventQueue() 
{

    timer = al_create_timer(1.0/Window_FPS());
    
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(GetDisplay()));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

}

void CleanUpGameThreads()
{

    StopSimulation();

}

void CleanUp() 
{

    al_destroy_event_queue(event_queue);
    CleanUpRendering();
    CleanUpLogging();
    CleanUpGameThreads();

}

