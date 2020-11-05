#ifndef ACCOUNT_H
#define ACCOUNT_H

void InitAccount();
unsigned int GetSaveSeed();
int GetSaveId();
void CreateNewSave(char *save_name, char *player_name);
void Save();
void LoadSave(int load_save_id, int save_player_id);
void StorePlayerData();
int GetCurrentPlayerId();

void SetGameTime(time_t time_to_set);

time_t GetGameTime();
char *GetDate();

void Account_SetInGameStatus(int status);
int Account_GetInGameStatus();

void SetGameSpeed(const int speed);
bool CanMakeTransaction(float amount);
void AddMoney(float amount);
void SubtractMoney(float amount);
void SetAccountMoney(float amount);
float GetAccountMoney();

char *GetCurrentSaveName();
char *GetCurrentPlayerName();

#endif