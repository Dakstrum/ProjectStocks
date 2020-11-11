
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
#include "game.h"

static atomic_uint game_seed;
static atomic_int  save_id;
static atomic_int  player_id;

static float account_money = 0;
static int in_game = 0;

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

void Account_Init() 
{

    atomic_store(&save_id, -1);
    atomic_store(&game_seed, 0);

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
    Game_SetGameTime(3600);
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
    Game_SetGameTime(save.time_spent_in_game);

    account_money = save.save_player_money;

    InitializeAccountInformation();
    InitializeCardInformation();

}

void Account_StorePlayerData() 
{

    PlayerSave save;
    save.save_name          = current_save_name;
    save.save_player_name   = current_player_name;
    save.time_spent_in_game = Game_GetGameTime();
    save.game_seed          = game_seed;
    save.save_player_id     = player_id;
    save.save_id            = save_id;
    save.save_player_money  = account_money;

    SavePlayerData(save);
    SaveTransactions();

}

char *Account_GetSaveName() 
{

    return current_save_name;

}

char *Account_GetPlayerName()
{

    return current_player_name;
    
}