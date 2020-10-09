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
        *((int *)save_id) = (unsigned int)atoi(argv[0]);

    return 0;

}

int InsertPlayerEntry(int save_id, char *player_name, double money, int save_owner)
{
    int player_id = -1;
    sqlite3 *db;
    if (OpenConnection(&db, DefaultConnection()) == 0) {

        ExecuteQuery(GetFormattedPointer("INSERT INTO Game_Players (SaveId, PlayerName, Money, SaveOwner) VALUES (%d, '%s', %.14f, %d)", save_id, player_name, money, save_owner), NULL, NULL, db);
        player_id = sqlite3_last_insert_rowid(db);

    }
    sqlite3_close(db);
    return player_id;
}

int InsertSaveEntry(char *save_name, unsigned int game_seed)
{
    int save_id = -1;
    sqlite3 *db;
    if (OpenConnection(&db, DefaultConnection()) == 0) {

        ExecuteQuery(GetFormattedPointer("INSERT INTO Game_Saves (SaveName, RandomSeed) VALUES ('%s', %d)", save_name, game_seed), NULL, NULL, db);
        save_id = sqlite3_last_insert_rowid(db);

    }
    sqlite3_close(db);
    return save_id;
}

void DeleteSave(int save_id)
{

    sqlite3 *db;
    if (OpenConnection(&db, DefaultConnection()) == 0)
        ExecuteQuery(GetFormattedPointer("DELETE FROM Game_Players WHERE SaveId = %d; DELETE FROM Saves WHERE SaveId = %d;", save_id, save_id), NULL, NULL, db);
    
    sqlite3_close(db);

}


int GetSaveSeedCallback(void *seed, int argc, char **argv, char **col_name) 
{

    if (argc > 0)
        *((unsigned int *)seed) = (unsigned int)atoi(argv[0]);

    return 0;

}

unsigned int GetSaveSeedWithSaveId(int save_id)
{

    unsigned int seed = 0;

    sqlite3 *db;
    if (OpenConnection(&db, DefaultConnection()) == 0)
        ExecuteQuery(GetFormattedPointer("SELECT RandomSeed FROM Game_Saves WHERE SaveId=%d", save_id), &GetSaveSeedCallback, &seed, db);

    sqlite3_close(db);

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

    sqlite3 *db;
    if (OpenConnection(&db, DefaultConnection()) == 0)
        ExecuteQuery(GetFormattedPointer("SELECT SaveName FROM Game_Saves WHERE SaveId = %d", save_id), &GetSaveNameFromSaveIdCallback, &save_name, db);

    sqlite3_close(db);

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

    char *player_name = malloc(sizeof(char) * 128);

    sqlite3 *db;
    if (OpenConnection(&db, DefaultConnection()) == 0)
        ExecuteQuery(GetFormattedPointer("SELECT PlayerName FROM Game_Saves WHERE SaveName = '%s'", save_name), &GetPlayerNameFromSaveNameCallback, &player_name, db);

    sqlite3_close(db);

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
    save.save_name          = malloc(32);
    save.save_player_name   = malloc(32);

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

    PlayerSave *temp = (PlayerSave *)save;

    temp->save_id            = atoi(argv[0]);
    temp->time_spent_in_game = atoi(argv[2]);
    temp->game_seed          = atoi(argv[3]);
    temp->save_player_id     = atoi(argv[4]);
    temp->save_player_money  = atof(argv[6]);
    temp->save_name          = malloc(32);
    temp->save_player_name   = malloc(32);

    strncpy(temp->save_name, argv[1], 32);
    strncpy(temp->save_player_name, argv[5], 32);

    temp->save_name[31]        = '\0';
    temp->save_player_name[31] = '\0';

    return 0;

}


PlayerSave GetSaveData(int save_id)
{

    PlayerSave save;
    char *query = "SELECT S.SaveId, S.SaveName, S.TimeSpentInGame, S.RandomSeed, P.PlayerId, P.PlayerName, P.Money FROM Game_Saves S "
                  "INNER JOIN Game_Players P ON P.SaveId = S.SaveId "
                  "WHERE P.SaveOwner = 1 AND S.SaveId = %d";
    ExecuteQueryF(&GetSaveData_Callback, &save, query, save_id);
    return save;

}

void SavePlayerData(PlayerSave save)
{

    char *query = "UPDATE Game_Saves SET TimeSpentInGame = %d WHERE SaveId = %d;"
                  "UPDATE Game_Players SET Money = %f WHERE PlayerId = %d;";

    ExecuteQueryF(NULL, NULL, query, save.time_spent_in_game, save.save_id, save.save_player_money, save.save_player_id);

}