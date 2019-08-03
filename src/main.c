#include <stdio.h>

#include <allegro5/allegro.h>

#include "controls.h"
#include "rendering.h"

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

    if (!Initialize()) {

        StartInputLoop();

    }
    CleanUp();

}

enum InitializeSuccess Initialize() 
{

    if (!al_init()) {

        fprintf(stderr, "failed to initialize allegro");
        return FAILURE;

    }
    InitializeThreads();
    return SUCCESS;

}



static ALLEGRO_THREAD *stock_simulation_thread = NULL;
static ALLEGRO_THREAD *rendering_thread        = NULL;
static ALLEGRO_THREAD *audio_thread            = NULL;
void InitializeThreads() 
{



}

void CleanUpThreads() 
{

    al_destroy_thread(stock_simulation_thread);
    al_destroy_thread(rendering_thread);
    al_destroy_thread(audio_thread);

}

void CleanUp() 
{

    CleanUpThreads();

}

