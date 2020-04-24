#ifndef DBSAVE_H
#define DBSAVE_H

#include "vector.h"

typedef struct PlayerSave {

    int save_id;
    int save_player_id;
    char *save_name;
    char *save_player_name;
    double save_player_money;
    unsigned int time_spent_in_game;
    unsigned int game_seed;

} PlayerSave;

unsigned int GetSaveSeedWithSaveId(int save_id);
void DeleteSave(int save_id);

int InsertSaveEntry(char *save_name, unsigned int game_seed);
int InsertPlayerEntry(int save_id, char *player_name, double money, int save_owner);

Vector *GetAllSaves();

#endif