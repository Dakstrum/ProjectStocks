#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int GetAllSavesCallback(void *saves, int argc, char **argv, char **col_name)
{

    if (argc == 0)
        return -1;

    LogF("retrieved %d elements", argc);
    LogF("%s %s %s %s %s %s %s", argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);
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

    PushBack(temp_vec, &save);
    return 0;

}

Vector *GetAllSaves() 
{
    Vector *saves = CreateVector(sizeof(PlayerSave), 16);
    sqlite3 *db;
    char *query = "SELECT S.SaveId, S.SaveName, S.TimeSpentInGame, S.RandomSeed, P.PlayerId, P.PlayerName, P.Money FROM Saves S "
                  "INNER JOIN Players P ON P.SaveId = S.SaveId "
                  "WHERE P.SaveOwner = 1";

    if (OpenConnection(&db, DefaultConnection()) == 0)
        ExecuteQuery(GetFormattedPointer(query), &GetAllSavesCallback, saves, db);

    sqlite3_close(db);

    return saves;

}