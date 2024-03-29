
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

static int in_game = 0;
static Save current_save;


void Account_SetInGameStatus(int status)
{

    in_game = status;

}

int Account_GetInGameStatus()
{

    return in_game;

}

Player *Account_GetPlayer(uint32_t player_id)
{

    Player *players = current_save.players->elements;
    for (size_t i = 0; i < current_save.players->num_elements;i++)
        if (players[i].player_id == player_id)
            return &players[i];

    assert(0);
    return NULL;
    
}

bool Account_CanMakeTransaction(uint32_t player_id, float amount) 
{

    Player *player = Account_GetPlayer(player_id);
    return player->money - amount >= 0.0;

}

void Account_AddMoney(uint32_t player_id, float amount)
{

    Player *player = Account_GetPlayer(player_id);
    player->money += amount;

}

void Account_SubtractMoney(uint32_t player_id, float amount)
{

    Player *player = Account_GetPlayer(player_id);
    player->money -= amount;

}

void Account_SetMoney(uint32_t player_id, float amount)
{

    Player *player = Account_GetPlayer(player_id);
    player->money  = amount;

}

float Account_GetMoney(uint32_t player_id)
{

    Player *player = Account_GetPlayer(player_id);
    return player->money;

}

char *Account_GetPlayerName(uint32_t player_id)
{

    Player *player = Account_GetPlayer(player_id);
    return player->name;
    
}

Vector *Account_GetPlayers()
{

    return current_save.players;

}

uint32_t Account_GetPlayerId()
{

    Player *players = current_save.players->elements;
    for (size_t i = 0; i < current_save.players->num_elements;i++)
        if (players[i].save_owner)
            return players[i].player_id;

    assert(0);
    return 0;
}

void Account_Init() 
{

    if (current_save.players != NULL)
        Vector_Delete(current_save.players);

    current_save.players = NULL;

}

int Account_CreateSaveEntries(char *save_name, char *player_name) 
{

    static const float STARTING_MONEY = 15000.0;

    int save_id = dbsave_insert_save(save_name, Game_GetSeed());

    dbsave_insert_player(save_id, player_name, STARTING_MONEY, 1);

    for (size_t i = 0; i < 3;i++)
        dbsave_insert_ai_player(save_id);

    return save_id;

}

void LoadSave(int load_save_id)
{

    Game_Reset();
    Game_SetSaveId(load_save_id);

    if (current_save.players != NULL)
        Vector_Delete(current_save.players);

    current_save = GetSaveData(load_save_id);

    Game_SetSeed(current_save.game_seed);
    Game_SetGameTime(current_save.time_spent_in_game);

    dbaccount_init();
    dbcard_init();
    Game_Init();

}

void Account_CreateSave(char *save_name, char *player_name)
{

    Game_Reset();
    Game_SetSeed(time(NULL));
    Game_SetGameTime(0);
    LoadSave(Account_CreateSaveEntries(save_name, player_name));

}

void Account_StorePlayerData() 
{

    current_save.time_spent_in_game = Game_GetGameTime();

    dbsave_save_player_data(current_save);
    dbaccount_save_transactions();
    dbcard_save_cards();

}

char *Account_GetSaveName() 
{

    return current_save.save_name;

}