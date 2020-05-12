
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdatomic.h>

#include <allegro5/allegro.h>

#include "log.h"
#include "shared.h"
#include "account.h"
#include "dbaccount.h"
#include "dbsave.h"

static atomic_long game_time;
static atomic_long game_time_real_dt;
static atomic_long game_time_game_dt;
static atomic_uint game_seed;
static atomic_bool pause_game_time;
static atomic_int  save_id;
static atomic_int  player_id;

static ALLEGRO_THREAD *account_thread = NULL;

static float account_money   = 0;
static int companyid_viewing = 1;
static int in_game           = 0;

static char *current_time_buf = NULL;
static const long ONE_HOUR = 3600;

static char *current_player_name = NULL;
static char *current_save_name   = NULL;

void *AccountEntry(ALLEGRO_THREAD *thread, void *arg);

void SetInGameStatus(int status)
{

    in_game = status;

}

int GetInGameStatus()
{

    return in_game;

}

void SetCompanyIdViewing(int id)
{

    companyid_viewing = id;

}

int GetCompanyIdViewing()
{

    return companyid_viewing;

}

void SetAccountMoney(float amount)
{

    account_money = amount;

}

float GetAccountMoney()
{

    return account_money;

}

char *GetCompanyNameViewing()
{

    return GetStockNameFromStockId(GetCompanyIdViewing());

}

int GetCurrentPlayerId()
{

    return atomic_load(&player_id);

}

int GetSaveId() 
{

    return atomic_load(&save_id);

}

unsigned int GetSaveSeed() 
{

    return atomic_load(&game_seed);

}

void InitAccount() 
{

    atomic_store(&save_id, -1);
    atomic_store(&game_time, ONE_HOUR);
    atomic_store(&game_time_real_dt, 2);
    atomic_store(&pause_game_time, false);
    atomic_store(&game_seed, 0);
    atomic_store(&game_time_game_dt, ONE_HOUR);

    account_thread = al_create_thread(&AccountEntry, NULL);
    al_start_thread(account_thread);

    current_time_buf    = malloc(128);
    current_save_name   = malloc(64);
    current_player_name = malloc(64);

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

    if (dt == atomic_load(&game_time_real_dt)) {

        atomic_store(&game_time, atomic_load(&game_time) + atomic_load(&game_time_game_dt));
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

void CreateNewSaveEntries(char *save_name, char *player_name) 
{
    atomic_store(&save_id, InsertSaveEntry(save_name, atomic_load(&game_seed)));
    if (atomic_load(&save_id) == -1) {

        Log("Unable to create save");
        return;

    }

    atomic_store(&player_id, InsertPlayerEntry(save_id, player_name, 3000.0, 1));

    if (atomic_load(&player_id) == -1)
        Log("Unable to create player");

}


void CreateNewSave(char *save_name, char *player_name)
{

    strncpy(current_save_name, save_name, 32);
    strncpy(current_player_name, player_name, 32);

    current_save_name[31]   = '\0';
    current_player_name[31] = '\0';

    atomic_store(&game_seed, time(NULL));
    CreateNewSaveEntries(save_name, player_name);

}

void LoadSave(int load_save_id, int save_player_id)
{

    atomic_store(&save_id, load_save_id);
    atomic_store(&player_id, save_player_id);

    PlayerSave save = GetSaveData(load_save_id);

    strncpy(current_save_name, save.save_name, 32);
    strncpy(current_player_name, save.save_player_name, 32);

    atomic_store(&game_seed, save.game_seed);
    atomic_store(&game_time, save.time_spent_in_game);

}

void Save() 
{



}

void SetGameSpeed(const int speed) 
{

    static const long TWO_HOURS  = ONE_HOUR * 2;
    static const long FOUR_HOURS = ONE_HOUR * 4;

    switch (speed) {

        case 2:  atomic_store(&game_time_game_dt, TWO_HOURS); break;
        case 3:  atomic_store(&game_time_game_dt, FOUR_HOURS); break;
        default: atomic_store(&game_time_game_dt, ONE_HOUR); break;

    }

}


char *GetDate()
{

    time_t current_time = GetGameTime();
    strftime(current_time_buf, 128, "%HH %x", localtime(&current_time));
    Log(current_time_buf);
    return current_time_buf;

}

char *GetCurrentSaveName() 
{

    return current_save_name;

}

char *GetCurrentPlayerName()
{

    return current_player_name;
    
}