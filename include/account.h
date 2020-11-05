#ifndef ACCOUNT_H
#define ACCOUNT_H

void Account_Init();
unsigned int Account_GetSaveSeed();
int Account_GetSaveId();
void CreateNewSave(char *save_name, char *player_name);
void Save();
void LoadSave(int load_save_id, int save_player_id);
void Account_StorePlayerData();
int Account_GetPlayerId();

void SetGameTime(time_t time_to_set);

time_t Account_GetGameTime();
char *Account_GetDate();

void Account_SetInGameStatus(int status);
int Account_GetInGameStatus();

void SetGameSpeed(const int speed);
bool Account_CanMakeTransaction(float amount);
void Account_AddMoney(float amount);
void Account_SubtractMoney(float amount);
void Account_SetMoney(float amount);
float Account_GetMoney();

char *Account_GetSaveName();
char *Account_GetPlayerName();

#endif