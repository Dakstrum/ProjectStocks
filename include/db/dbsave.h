#ifndef DBSAVE_H
#define DBSAVE_H

int InsertSave(char *save_name, char *player_name, unsigned int game_seed);
unsigned int GetSaveSeedWithSaveId(int save_id);
void DeleteAccountSave(char *save_name, char *player_name);

int InsertSaveEntry(char *save_name, unsigned int game_seed);
int InsertPlayerEntry(int save_id, char *player_name, double money, int save_owner);

#endif