
#include <time.h>
#include <stdint.h>
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

void Account_Init() 
{


}

void CreateNewSaveEntries(char *save_name, char *player_name) 
{
    int new_save_id = InsertSaveEntry(save_name, Game_GetSeed());
    
    if (new_save_id == -1) {

        Log("Unable to create save");
        return;

    }
    Game_SetSaveId(new_save_id);

    atomic_store(&player_id, InsertPlayerEntry(new_save_id, player_name, account_money, 1));

    if (atomic_load(&player_id) == -1)
        Log("Unable to create player");

    for (size_t i = 0; i < 3;i++)
        InsertAIPlayerEntry(new_save_id);

}


void CreateNewSave(char *save_name, char *player_name)
{

    Game_Reset();

    account_money = 15000.0;
    strncpy(current_save_name, save_name, 32);
    strncpy(current_player_name, player_name, 32);

    current_save_name[31]   = '\0';
    current_player_name[31] = '\0';

    Game_SetSeed(time(NULL));
    Game_SetGameTime(3600);
    CreateNewSaveEntries(save_name, player_name);

    InitializeAccountInformation();
    InitializeCardInformation();

    Game_Init();

}

void LoadSave(int load_save_id, int save_player_id)
{

    Game_SetSaveId(load_save_id);
    atomic_store(&player_id, save_player_id);

    PlayerSave save = GetSaveData(load_save_id);

    strncpy(current_save_name, save.save_name, 32);
    strncpy(current_player_name, save.save_player_name, 32);

    Game_SetSeed(save.game_seed);
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
    save.game_seed          = Game_GetSeed();
    save.save_player_id     = player_id;
    save.save_id            = Game_GetSaveId();
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