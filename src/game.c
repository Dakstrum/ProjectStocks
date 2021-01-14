
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdatomic.h>

#include <allegro5/allegro.h>

#include "shared.h"
#include "timer.h"
#include "game.h"
#include "simulation.h"

static atomic_long game_time;
static atomic_uint game_seed;
static atomic_int  save_id;

static ALLEGRO_THREAD *game_thread = NULL;

static float sleep_time = 1.0;
static const long ONE_HOUR = 3600;
static const long SIX_HOURS = ONE_HOUR * 6;

static char current_time_buf[128];


bool Game_TryIncrement(long int dt) 
{

    if (dt == 2) {

        Simulation_SimulateStep(atomic_load(&game_time) + SIX_HOURS);
        atomic_store(&game_time, atomic_load(&game_time) + SIX_HOURS);
        return true;

    }
    return false;

}

void Game_InitSimulation()
{

    Simulation_Init(atomic_load(&game_seed));
    Simulation_SimulateUntil(game_time);

}

void *Game_Entry(ALLEGRO_THREAD *thread, void *arg) 
{

    Game_InitSimulation();

    long int dt = 0;
    while (!ShouldICleanUp()) {

        al_rest(sleep_time);
        if (Timer_IsPaused())
            continue;

        dt += 1;
        if (Game_TryIncrement(dt))
            dt = 0;

    }
    return NULL;

}


void Game_Init()
{

    game_thread = al_create_thread(Game_Entry, NULL);
    al_start_thread(game_thread);

}

void Game_Reset()
{

    if (game_thread == NULL)
        return;

    al_join_thread(game_thread, NULL);
    al_destroy_thread(game_thread);
    game_thread = NULL;

}

char *Game_GetDate()
{

    time_t current_time = Game_GetGameTime();
    strftime(current_time_buf, 128, "%HH %x", localtime(&current_time));
    
    return current_time_buf;

}

void Game_SetSeed(uint32_t new_seed) 
{

    atomic_store(&game_seed, new_seed);

}

uint32_t Game_GetSeed()
{

    return atomic_load(&game_seed);

}

void Game_SetSaveId(uint32_t new_save_id) 
{

    atomic_store(&save_id, new_save_id);

}

uint32_t Game_GetSaveId()
{

    return atomic_load(&save_id);

}

time_t Game_GetGameTime()
{

    return atomic_load(&game_time);

}

void Game_SetGameTime(time_t new_game_time)
{

    atomic_store(&game_time, new_game_time);

}

void Game_SetSpeed(const int speed)
{

    switch (speed) {

        case 2:  sleep_time = .5; break;
        case 3:  sleep_time = .1; break;
        default: sleep_time = 1.5; break;

    }

}