#define ALLEGRO_USE_CONSOLE 1

#include <stdio.h>
#include <allegro5/allegro.h>

#include "dbaccess.h"
#include "shared.h"
#include "controls.h"
#include "rendering.h"
#include "simulation.h"

enum InitializeSuccess 
{

    FAILURE = 0,
    SUCCESS

};

enum InitializeSuccess Initialize();
void InitializeThreads();
void CleanUp();
void CleanUpThreads();
void StartInputLoop();

int main(int argc, char **argv) 
{

    printf("Initialize\n");
    if (Initialize()) {

        StartInputLoop();

    }
    CleanUp();
    return 0;

}

enum InitializeSuccess Initialize() 
{

    if (!al_init()) {

        fprintf(stderr, "failed to initialize allegro");
        return FAILURE;

    }
    InitializeDatabases();
    InitializeThreads();
    return SUCCESS;

}



static ALLEGRO_THREAD *stock_simulation_thread = NULL;
static ALLEGRO_THREAD *rendering_thread        = NULL;
static ALLEGRO_THREAD *audio_thread            = NULL;
void InitializeThreads() 
{

    stock_simulation_thread = al_create_thread(StockSimulationEntry, NULL);
    rendering_thread        = al_create_thread(RenderingEntry, NULL);

    al_start_thread(stock_simulation_thread);
    al_start_thread(rendering_thread);

}

void CleanUpThreads() 
{

    al_join_thread(stock_simulation_thread, NULL);
    al_join_thread(rendering_thread, NULL);
    //al_join_thread(audio_thread, NULL);

    al_destroy_thread(stock_simulation_thread);
    al_destroy_thread(rendering_thread);
    //al_destroy_thread(audio_thread);

}

void CleanUp() 
{

    CleanUpThreads();

}

