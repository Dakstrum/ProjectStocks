#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sqlite3.h>

#include "log.h"
#include "dbsave.h"
#include "dbaccess.h"
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

        ExecuteQuery(GetFormattedPointer("INSERT INTO Players (SaveId, PlayerName, Money, SaveOwner) VALUES (%d, '%s', %.14f, %d)", save_id, player_name, money, save_owner), NULL, NULL, db);
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

        ExecuteQuery(GetFormattedPointer("INSERT INTO Saves (SaveName, RandomSeed) VALUES ('%s', %d)", save_name, game_seed), NULL, NULL, db);
        save_id = sqlite3_last_insert_rowid(db);

    }
    sqlite3_close(db);
    return save_id;
}

void DeleteAccountSave(char *save_name, char *player_name)
{

    sqlite3 *db;
    if (OpenConnection(&db, DefaultConnection()) == 0)
        ExecuteQuery(GetFormattedPointer("DELETE FROM Saves WHERE SaveName = '%s' AND PlayerName = '%s'", save_name, player_name), NULL, NULL, db);

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
        ExecuteQuery(GetFormattedPointer("SELECT RandomSeed FROM Saves WHERE SaveId=%d", save_id), &GetSaveSeedCallback, &seed, db);

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
        ExecuteQuery(GetFormattedPointer("SELECT SaveName FROM Saves WHERE SaveId = %d", save_id), &GetSaveNameFromSaveIdCallback, &save_name, db);

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
        ExecuteQuery(GetFormattedPointer("SELECT PlayerName FROM Saves WHERE SaveName = '%s'", save_name), &GetPlayerNameFromSaveNameCallback, &player_name, db);

    sqlite3_close(db);

    return player_name;

}


void ResizePlayerSavesElements(PlayerSaves *saves)
{

    saves->size += 16;
    saves->save_id            = realloc(saves->save_id, sizeof(int) * saves->size);
    saves->save_player_id     = realloc(saves->save_player_id, sizeof(int) * saves->size);
    saves->save_name          = realloc(saves->save_name, sizeof(char *) * saves->size);
    saves->save_player_name   = realloc(saves->save_player_name, sizeof(char *) * saves->size);
    saves->save_player_money  = realloc(saves->save_player_money, sizeof(double) * saves->size);
    saves->time_spent_in_game = realloc(saves->time_spent_in_game, sizeof(unsigned int) * saves->size);
    saves->game_seed          = realloc(saves->game_seed, sizeof(unsigned int) * saves->size);

}

int GetAllSavesCallback(void *saves, int argc, char **argv, char **col_name)
{

    if (argc == 0)
        return -1;

    PlayerSaves *temp = (PlayerSaves *)saves;

    if (temp->num_saves == temp->size)
        ResizePlayerSavesElements(temp);

    temp->save_id[temp->num_saves]            = atoi(argv[0]);
    temp->save_name[temp->num_saves]          = NULL;
    temp->time_spent_in_game[temp->num_saves] = atoi(argv[2]);
    temp->game_seed[temp->num_saves]          = atoi(argv[3]);
    temp->save_player_id[temp->num_saves]     = atoi(argv[4]);
    temp->save_player_name[temp->num_saves]   = NULL;
    temp->save_player_money[temp->num_saves]  = atof(argv[6]);

    return 0;

}

PlayerSaves *GetAllSaves() 
{
    PlayerSaves *saves = malloc(sizeof(PlayerSaves));

    saves->num_saves          = 0;
    saves->size               = 16;
    saves->save_id            = malloc(sizeof(int) * 16);
    saves->save_player_id     = malloc(sizeof(int) * 16);
    saves->save_name          = malloc(sizeof(char *) * 16);
    saves->save_player_name   = malloc(sizeof(char *) * 16);
    saves->save_player_money  = malloc(sizeof(double) * 16);
    saves->time_spent_in_game = malloc(sizeof(unsigned int) * 16);
    saves->game_seed          = malloc(sizeof(unsigned int) * 16);

    sqlite3 *db;
    char *query = "SELECT S.SaveId, S.SaveName, S.TimeSpentInGame, S.RandomSeed, P.PlayerId, P.PlayerName, P.Money FROM Saves S "
                  "INNER JOIN Players P ON P.SaveId = S.SaveId "
                  "WHERE P.SaveOwner = 1";

    if (OpenConnection(&db, DefaultConnection()) == 0)
        ExecuteQuery(GetFormattedPointer(query), &GetAllSavesCallback, saves, db);

    sqlite3_close(db);

    return saves;

}
