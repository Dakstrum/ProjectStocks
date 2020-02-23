#include <stdio.h>

#ifdef __linux__
    #include <signal.h>
#endif
#include <allegro5/allegro.h>

#include "dbaccess.h"
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

enum InitializeSuccess 
{

    FAILURE = 0,
    SUCCESS

};
/* Test code */
static ALLEGRO_EVENT_QUEUE *event_queue;
static ALLEGRO_TIMER *timer;

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
    if (event.type != ALLEGRO_EVENT_TIMER) {

        HandleInput(event);
        HandleWindowEvents(event);

    } else {

        HandleMouseLocation();
        HandleRendering();

    }

}

void GameLoop() 
{

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
    InitializeCache();
    InitializeShared();
    InitializeDatabases();
    InitializeControls();
    InitializeRendering();
    InitializeEventQueue();
    InitializeJson();
    InitializeAudio();
    //CopyPersistentToMemory();
    InitAccount();
    InitializeSimulation();
    InitializeGameThreads();

    return SUCCESS;

}

void InitializeEventQueue() 
{

    LogF("Display modes found = %d", al_get_num_display_modes());
    timer = al_create_timer(1.0/60.0);
    
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(GetDisplay()));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

}


void InitializeGameThreads() 
{


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

