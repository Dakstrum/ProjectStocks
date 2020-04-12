#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sqlite3.h>

#include "log.h"
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

int InsertSave(char *save_name, char *player_name, unsigned int game_seed)
{

    int save_id = InsertSaveEntry(save_name, game_seed);
    if (save_id == -1) {

        Log("Unable to create save");
        return -1;

    }

    int player_id = InsertPlayerEntry(save_id, player_name, 3000.0, 1);
    if (player_id == -1) {

        Log("Unable to create player");
        return -1;

    }

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
