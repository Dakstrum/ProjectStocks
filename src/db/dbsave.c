#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sqlite3.h>

#include "log.h"
#include "dbsave.h"
#include "shared.h"
#include "dbutils.h"
#include "account.h"

int GetSaveIdCallback(void *save_id, int argc, char **argv, char **col_name) 
{

    if (argc > 0)
        *((int *)save_id) = atoi(argv[0]);

    return 0;

}

void dbsave_insert_ai_player(int save_id) 
{

    char *query =   "INSERT INTO Game_Players (SaveId, PlayerName, Money, SaveOwner) "
                    "SELECT "
                    "%d,"
                    "(SELECT FirstName FROM System_AIFirstnames ORDER BY RANDOM() LIMIT 1) || ' ' || (SELECT LastName FROM System_AILastNames ORDER BY RANDOM() LIMIT 1),"
                    "15000,"
                    "0";

    ExecuteQueryF(NULL, NULL, query, save_id);

}

int dbsave_insert_player(int save_id, char *player_name, double money, int save_owner)
{

    sqlite3 *db = GetOpenConnection(DefaultConnection());

    ExecuteQueryFDB(NULL, NULL, db, "INSERT INTO Game_Players (SaveId, PlayerName, Money, SaveOwner) VALUES (%d, '%s', %.14f, %d)", save_id, player_name, money, save_owner);
    int player_id = sqlite3_last_insert_rowid(db);

    sqlite3_close(db);

    return player_id;
}

int dbsave_insert_save(char *save_name, unsigned int game_seed)
{

    sqlite3 *db = GetOpenConnection(DefaultConnection());

    ExecuteQueryFDB(NULL, NULL, db, "INSERT INTO Game_Saves (SaveName, RandomSeed) VALUES ('%s', %d)", save_name, game_seed);
    int save_id = sqlite3_last_insert_rowid(db);

    sqlite3_close(db);

    return save_id;

}

void dbsave_delete_save(int save_id)
{

    ExecuteQueryF(NULL, NULL, "DELETE FROM Game_Players WHERE SaveId = %d; DELETE FROM Saves WHERE SaveId = %d;", save_id, save_id);

}


int GetSaveSeedCallback(void *seed, int argc, char **argv, char **col_name) 
{

    if (argc > 0)
        *((uint32_t *)seed) = (uint32_t)atoi(argv[0]);

    return 0;

}

unsigned int GetSaveSeedWithSaveId(int save_id)
{

    unsigned int seed = 0;

    ExecuteQueryF(&GetSaveSeedCallback, &seed, "SELECT RandomSeed FROM Game_Saves WHERE SaveId=%d", save_id);

    return seed;

}

int GetSaveNameFromSaveIdCallback(void *save_name, int argc, char **argv, char **col_name)
{

    if (argc > 0) {

        char *temp = *((char **)save_name);
        strncpy(temp, argv[0], 127);
        temp[127] = '\0';

    }

    return 0;

}

char *GetSaveNameFromSaveId(int save_id)
{

    char *save_name = malloc(sizeof(char) * 128);

    ExecuteQueryF(&GetSaveNameFromSaveIdCallback, &save_name, "SELECT SaveName FROM Game_Saves WHERE SaveId = %d", save_id);

    return save_name;

}

int GetPlayerNameFromSaveNameCallback(void *player_name, int argc, char **argv, char **col_name)
{

    if (argc > 0) {

        char *temp = *((char **)player_name);
        strncpy(temp, argv[0], 127);
        temp[127] = '\0';

    }

    return 0;

}

char *GetPlayerNameFromSaveName(char *save_name)
{

    char *player_name = malloc(128);

    ExecuteQueryF(&GetPlayerNameFromSaveNameCallback, &player_name, "SELECT PlayerName FROM Game_Saves WHERE SaveName = '%s'", save_name);

    return player_name;

}

int GetAllSaves_Callback(void *saves, int argc, char **argv, char **col_name)
{

    if (argc == 0)
        return -1;

    Vector *temp_vec = (Vector *)saves;
    PlayerSave save;
    save.save_id            = atoi(argv[0]);
    save.time_spent_in_game = atoi(argv[2]);
    save.game_seed          = atoi(argv[3]);
    save.save_player_id     = atoi(argv[4]);
    save.save_player_money  = atof(argv[6]);

    strncpy(save.save_name, argv[1], 32);
    strncpy(save.save_player_name, argv[5], 32);

    save.save_name[31]        = '\0';
    save.save_player_name[31] = '\0';

    Vector_PushBack(temp_vec, &save);
    return 0;

}

Vector *GetAllSaves() 
{
    Vector *saves = Vector_Create(sizeof(PlayerSave), 16);
    char *query   = "SELECT S.SaveId, S.SaveName, S.TimeSpentInGame, S.RandomSeed, P.PlayerId, P.PlayerName, P.Money FROM Game_Saves S "
                    "INNER JOIN Game_Players P ON P.SaveId = S.SaveId "
                    "WHERE P.SaveOwner = 1";

    ExecuteQueryF(&GetAllSaves_Callback, saves, query);
    return saves;

}

int GetSaveData_Callback(void *save, int argc, char **argv, char **col_name)
{

    if (argc == 0)
        return 0;

    Save *temp = (Save *)save;

    strncpy(temp->save_name, argv[1], 32);
    temp->save_name[31] = '\0';

    temp->save_id            = atoi(argv[0]);
    temp->time_spent_in_game = atoi(argv[2]);
    temp->game_seed          = atoi(argv[3]);

    return 0;

}

int GetPlayerSaveData_Callback(void *player, int argc, char **argv, char **col_name) 
{

    if (argc == 0)
        return 0;

    Player temp;
    temp.player_id = atoi(argv[0]);

    strncpy(temp.name, argv[1], 32);
    temp.name[31] = '\0';

    temp.money = atof(argv[2]);
    temp.save_owner = atoi(argv[3]) == 1;

    Vector_PushBack(player, &temp);

    return 0;

}


Save GetSaveData(int save_id)
{

    Save save;
    save.players = Vector_Create(sizeof(Player), 4);

    char *save_query = "SELECT S.SaveId, S.SaveName, S.TimeSpentInGame, S.RandomSeed FROM Game_Saves S WHERE S.SaveId = %d";
    ExecuteQueryF(&GetSaveData_Callback, &save, save_query, save_id);

    char *player_query = "SELECT P.PlayerId, P.PlayerName, P.Money, P.SaveOwner FROM Game_Players P WHERE P.SaveId = %d";
    ExecuteQueryF(&GetPlayerSaveData_Callback, save.players, player_query, save_id);

    return save;

}

void dbsave_save_player_data(Save save)
{

    ExecuteQueryF(NULL, NULL, "UPDATE Game_Saves SET TimeSpentInGame = %d WHERE SaveId = %d;", save.time_spent_in_game, save.save_id);

    Player *players = save.players->elements;
    for (size_t i = 0; i < save.players->num_elements;i++)
        ExecuteQueryF(NULL, NULL, "UPDATE Game_Players SET Money = %f WHERE PlayerId = %d;", players[i].money, players[i].player_id);


}