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
#include "account.h"

#include "windows.h"

enum InitializeSuccess 
{

    FAILURE = 0,
    SUCCESS

};

static ALLEGRO_EVENT_QUEUE *event_queue;
static ALLEGRO_TIMER *timer;
static int refresh_rate;

enum InitializeSuccess Initialize();
void GameLoop();
void InitializeGameThreads();
void InitializeEventQueue();
ALLEGRO_EVENT WaitForEvent();
void CleanUp();
void CleanUpThreads();
void StartInputLoop();
void GetRefreshRateOfMonitor();

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
    InitializeGraphCaching();
    InitializeCache();
    InitializeShared();
    InitializeDatabases();
    InitializeControls();
    InitializeRendering();
    InitializeEventQueue();
    InitializeJson();
    InitAccount();
    InitializeGameThreads();
    return SUCCESS;

}

void InitializeEventQueue() 
{
    GetRefreshRateOfMonitor();

    timer       = al_create_timer(1.0/refresh_rate);
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

void GetRefreshRateOfMonitor() //Im aware this is only for windows. Im looking into it
{
    DEVMODE* monitorProperties = NULL;
    monitorProperties = (DEVMODE*)malloc(sizeof(DEVMODE));
    EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, monitorProperties);
    LogF("ref = %d", monitorProperties->dmDisplayFrequency);
    refresh_rate = monitorProperties->dmDisplayFrequency;
}

