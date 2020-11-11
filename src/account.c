
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdatomic.h>

#include <allegro5/allegro.h>

#include "log.h"
#include "shared.h"
#include "account.h"
#include "dbcompany.h"
#include "dbaccount.h"
#include "dbsave.h"
#include "dbcard.h"

#include "timer.h"

static atomic_long game_time;
static atomic_long game_time_real_dt;
static atomic_long game_time_game_dt;
static atomic_uint game_seed;
static atomic_int  save_id;
static atomic_int  player_id;

static ALLEGRO_THREAD *account_thread = NULL;

static float sleep_time = 1.0;
static float account_money = 0;
static int in_game = 0;

static const long ONE_HOUR = 3600;

static char current_time_buf[128];
static char current_player_name[64];
static char current_save_name[64];


void Account_SetInGameStatus(int status)
{

    in_game = status;

}

int Account_GetInGameStatus()
{

    return in_game;

}

bool Account_CanMakeTransaction(float amount) 
{

    return account_money - amount >= 0.0;

}

void Account_AddMoney(float amount)
{

    account_money += amount;

}

void Account_SubtractMoney(float amount)
{

    account_money -= amount;

}

void Account_SetMoney(float amount)
{

    account_money = amount;

}

float Account_GetMoney()
{

    return account_money;

}

int Account_GetPlayerId()
{

    return atomic_load(&player_id);

}

int Account_GetSaveId() 
{

    return atomic_load(&save_id);

}

unsigned int Account_GetSaveSeed() 
{

    return atomic_load(&game_seed);

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

        al_rest(sleep_time);
        if (Timer_IsPaused())
            continue;

        

        dt += 1;
        if (CheckToIncrementGametime(dt))
            dt = 0;

    }
    return NULL;

}

void Account_Init() 
{

    atomic_store(&save_id, -1);
    atomic_store(&game_time, ONE_HOUR);
    atomic_store(&game_time_real_dt, 2);
    atomic_store(&game_seed, 0);
    atomic_store(&game_time_game_dt, ONE_HOUR);

    account_thread = al_create_thread(&AccountEntry, NULL);
    al_start_thread(account_thread);

}

time_t Account_GetGameTime()
{

    return atomic_load(&game_time);

}

void SetGameTime(time_t time_to_set) 
{

    atomic_store(&game_time, time_to_set);

}

void CreateNewSaveEntries(char *save_name, char *player_name) 
{
    atomic_store(&save_id, InsertSaveEntry(save_name, atomic_load(&game_seed)));
    if (atomic_load(&save_id) == -1) {

        Log("Unable to create save");
        return;

    }

    atomic_store(&player_id, InsertPlayerEntry(save_id, player_name, account_money, 1));

    if (atomic_load(&player_id) == -1)
        Log("Unable to create player");

    for (size_t i = 0; i < 3;i++)
        InsertAIPlayerEntry(save_id);

}


void CreateNewSave(char *save_name, char *player_name)
{

    account_money = 15000.0;
    strncpy(current_save_name, save_name, 32);
    strncpy(current_player_name, player_name, 32);

    current_save_name[31]   = '\0';
    current_player_name[31] = '\0';

    atomic_store(&game_seed, time(NULL));
    atomic_store(&game_time, ONE_HOUR);
    CreateNewSaveEntries(save_name, player_name);

    InitializeAccountInformation();
    InitializeCardInformation();

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

    account_money = save.save_player_money;

    InitializeAccountInformation();
    InitializeCardInformation();

}

void Account_StorePlayerData() 
{

    PlayerSave save;
    save.save_name          = current_save_name;
    save.save_player_name   = current_player_name;
    save.time_spent_in_game = atomic_load(&game_time);
    save.game_seed          = game_seed;
    save.save_player_id     = player_id;
    save.save_id            = save_id;
    save.save_player_money  = account_money;

    SavePlayerData(save);
    SaveTransactions();

}

void SetGameSpeed(const int speed) 
{

    switch (speed) {


        case 2:  sleep_time = .5; break;
        case 3:  sleep_time = .1; break;
        default: sleep_time = 1.5; break;

    }

}


char *Account_GetDate()
{

    time_t current_time = Account_GetGameTime();
    strftime(current_time_buf, 128, "%HH %x", localtime(&current_time));
    
    return current_time_buf;

}

char *Account_GetSaveName() 
{

    return current_save_name;

}

char *Account_GetPlayerName()
{

    return current_player_name;
    
}