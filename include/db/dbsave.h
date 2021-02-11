#ifndef DBSAVE_H
#define DBSAVE_H

#include <time.h>
#include <stdbool.h>
#include <stdint.h>

#include "vector.h"

typedef struct Player {

	uint32_t id;
	float money;
	char name[32];
	bool save_owner;

} Player;

typedef struct Save {

    int save_id;
    char save_name[32];
    Vector *players;
    time_t time_spent_in_game;
    uint32_t game_seed;

} Save;

typedef struct PlayerSave {

    int save_id;
    int save_player_id;
    char save_name[32];
    char save_player_name[32];
    double save_player_money;
    unsigned int time_spent_in_game;
    unsigned int game_seed;

} PlayerSave;


uint32_t GetSaveSeedWithSaveId(int save_id);
void DeleteSave(int save_id);

int InsertSaveEntry(char *save_name, uint32_t game_seed);
int InsertPlayerEntry(int save_id, char *player_name, double money, int save_owner);
void InsertAIPlayerEntry(int save_id);

void SavePlayerData(Save save);
Vector *GetAllSaves();
Save GetSaveData(int save_id);

#endif