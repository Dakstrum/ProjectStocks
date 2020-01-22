
#include <time.h>
#include <stdatomic.h>

#include <allegro5/allegro.h>

#include "log.h"
#include "shared.h"
#include "account.h"
#include "dbaccess.h"

static atomic_long game_time;
static atomic_long game_time_dt;
static atomic_uint game_seed;
static atomic_bool pause_game_time;
static atomic_int  save_id;

static ALLEGRO_THREAD *account_thread = NULL;

void *AccountEntry(ALLEGRO_THREAD *thread, void *arg);

void SaveLoadTest()
{

#if DEBUGGING

    CreateNewSave("NewDebugSave", "DebugEmma");
    atomic_store(&game_seed, 0);
    LoadSave(atomic_load(&save_id));

#endif

}

void InitAccount() 
{

    atomic_store(&save_id, -1);
    atomic_store(&game_time, 3600);
    atomic_store(&game_time_dt, 2);
    atomic_store(&pause_game_time, false);
    atomic_store(&game_seed, 0);

    account_thread = al_create_thread(&AccountEntry, NULL);
    al_start_thread(account_thread);
    SaveLoadTest();

}

int GetSaveId() 
{

    return atomic_load(&save_id);

}

unsigned int GetSaveSeed() 
{

    return atomic_load(&game_seed);

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


void CreateNewSave(char *save_name, char *player_name)
{

    unsigned int new_game_seed = time(NULL);
    atomic_store(&save_id, InsertSave(save_name, player_name, new_game_seed));
    atomic_store(&game_seed, new_game_seed);
    LogF("CreateNewSave save_id = %d, game_seed = %u", atomic_load(&save_id), atomic_load(&game_seed));

}

void DeleteSave(char *save_name, char *player_name)
{
    LogF("DeleteSave %s %s", save_name, player_name);
    //Todo delete save from DB with save_name and player_name

}

void LoadSave(int load_save_id)
{

    atomic_store(&save_id, load_save_id);
    atomic_store(&game_seed, GetSaveSeedWithSaveId(load_save_id));
    LogF("LoadSave game_seed = %u", atomic_load(&game_seed));

}