
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdatomic.h>

#include <allegro5/allegro.h>

#include "shared.h"
#include "timer.h"
#include "game.h"

static atomic_long game_time;
static atomic_long game_time_real_dt;
static atomic_long game_time_game_dt;

static ALLEGRO_THREAD *game_thread = NULL;

static float sleep_time = 1.0;
static const long ONE_HOUR = 3600;

static char current_time_buf[128];


bool Game_TryIncrement(long int dt) 
{

    if (dt == atomic_load(&game_time_real_dt)) {

        atomic_store(&game_time, atomic_load(&game_time) + atomic_load(&game_time_game_dt));
        return true;

    }
    return false;

}

void *Game_Entry(ALLEGRO_THREAD *thread, void *arg) 
{

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

void Game_Reset()
{

    atomic_store(&game_time, ONE_HOUR);
    atomic_store(&game_time_real_dt, 2);
    atomic_store(&game_time_game_dt, ONE_HOUR);

}


void Game_Init()
{

	Game_Reset();

    game_thread = al_create_thread(Game_Entry, NULL);
    al_start_thread(game_thread);

}

char *Game_GetDate()
{

    time_t current_time = Game_GetGameTime();
    strftime(current_time_buf, 128, "%HH %x", localtime(&current_time));
    
    return current_time_buf;

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