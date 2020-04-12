#ifndef DBSAVE_H
#define DBSAVE_H

typedef struct PlayerSaves {

    int *save_id;
    int *save_player_id;
    char **save_name;
    char **save_player_name;
    double *save_player_money;
    unsigned int *time_spent_in_game;
    unsigned int *game_seed;

    int num_saves;
    int size;

} PlayerSaves;

int InsertSave(char *save_name, char *player_name, unsigned int game_seed);
unsigned int GetSaveSeedWithSaveId(int save_id);
void DeleteAccountSave(char *save_name, char *player_name);

int InsertSaveEntry(char *save_name, unsigned int game_seed);
int InsertPlayerEntry(int save_id, char *player_name, double money, int save_owner);

PlayerSaves *GetAllSaves();

#endif