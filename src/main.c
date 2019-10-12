#define ALLEGRO_USE_CONSOLE 1

#include <stdio.h>
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

enum InitializeSuccess 
{

    FAILURE = 0,
    SUCCESS

};

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
    return 0;

}

void GameLoop() 
{

    while (!ShouldICleanUp()) {

        ALLEGRO_EVENT event = WaitForEvent();

        if (event.type != ALLEGRO_EVENT_TIMER) {

            HandleInput(event);
            HandleWindowEvents(event);

        } else {

            HandleRendering();

        }

    }

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
    InitializeGraphQueue();
    InitializeCache();
    InitializeShared();
    InitializeDatabases();
    InitializeControls();
    InitializeRendering();
    InitializeEventQueue();
    InitializeJson();
    InitializeGameThreads();
    return SUCCESS;

}

void InitializeEventQueue() 
{

    timer       = al_create_timer(1.0/60.0);
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(GetDisplay()));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

}


static ALLEGRO_THREAD *stock_simulation_thread = NULL;
static ALLEGRO_THREAD *audio_thread            = NULL;
void InitializeGameThreads() 
{

    stock_simulation_thread = al_create_thread(StockSimulationEntry, NULL);
    audio_thread            = al_create_thread(AudioEntry, NULL);

    al_start_thread(stock_simulation_thread);
    al_start_thread(audio_thread);

}

void CleanUpGameThreads()
{

    al_join_thread(stock_simulation_thread, NULL);
    al_join_thread(audio_thread, NULL);

    al_destroy_thread(stock_simulation_thread);
    al_destroy_thread(audio_thread);

}

void CleanUp() 
{

    al_destroy_event_queue(event_queue);
    CleanUpRendering();
    CleanUpGameThreads();
    al_uninstall_system();

}

