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
#include "animations.h"
#include "drawobject.h"

enum InitializeSuccess 
{

    FAILURE = 0,
    SUCCESS

};
/* Test code */
static ALLEGRO_EVENT_QUEUE *event_queue;
static ALLEGRO_TIMER *timer;

static struct timespec last_render_update;

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
    raise(SIGTSTP);
#endif
    return 0;

}

void Loop() 
{

    ALLEGRO_EVENT event = WaitForEvent();

    static ALLEGRO_EVENT input_event;
    static bool had_input = false;

    if (event.type == ALLEGRO_EVENT_TIMER) {

        struct timespec current_time = GetCurrentTime();

        Animate_Update(last_render_update, current_time);
        HandleMouseLocation();
        HandleRendering();

        if (had_input) {

            HandleInput(input_event);
            HandleWindowEvents(input_event); 
            
        }

        had_input          = false;
        last_render_update = current_time;

    } else {

        input_event = event;
        had_input   = true;

    }

}

void GameLoop() 
{

    last_render_update = GetCurrentTime();
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
    InitializeEventQueue();
    InitializeJson();
    InitializeAudio();
    InitAccount();
    InitializeSimulation();

    DrawObjects_Initialize();
    Animate_Initialize();

    return SUCCESS;

}

void InitializeEventQueue() 
{

    timer = al_create_timer(1.0/60.0);
    
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

