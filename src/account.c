
#include <time.h>
#include <stdatomic.h>

#include <allegro5/allegro.h>

#include "log.h"
#include "shared.h"

static int save_id = -1;
static atomic_long game_time;
static atomic_long game_time_dt;
static atomic_bool pause_game_time;

static ALLEGRO_THREAD *account_thread = NULL;

void *AccountEntry(ALLEGRO_THREAD *thread, void *arg);

void InitAccount() 
{

    atomic_store(&game_time, 3600);
    atomic_store(&game_time_dt, 2);
    atomic_store(&pause_game_time, false);

    account_thread = al_create_thread(&AccountEntry, NULL);
    al_start_thread(account_thread);

}

int GetSaveId() 
{

    return save_id;

}

unsigned int GetSaveSeed() 
{

    Log("STUB: GetSaveSeed");
    return 0;

}

void TogglePauseGameTime() 
{

    atomic_store(&pause_game_time, !atomic_load(&pause_game_time));

}

time_t GetGameTime() 
{

    return atomic_load(&game_time);

}

void SetGameTime(time_t time_to_set) 
{

    atomic_store(&game_time, time_to_set);

}

bool CheckToIncrementGametime(long int dt) 
{

    if (dt == atomic_load(&game_time_dt)) {

        atomic_store(&game_time, atomic_load(&game_time) + 3600);
        return true;

    }
    return false;

}

void *AccountEntry(ALLEGRO_THREAD *thread, void *arg) 
{

    long int dt = 0;
    while (!ShouldICleanUp()) {

        al_rest(1.0);
        if (atomic_load(&pause_game_time))
            continue;

        dt += 1;
        if (CheckToIncrementGametime(dt))
            dt = 0;


    }
    return NULL;

}
