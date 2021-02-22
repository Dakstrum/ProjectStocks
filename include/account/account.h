#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <stdbool.h>

#include "vector.h"

void Account_Init();
void Account_CreateSave(char *save_name, char *player_name);
void LoadSave(int load_save_id);
void Account_StorePlayerData();
uint32_t Account_GetPlayerId();

void Account_SetInGameStatus(int status);
int Account_GetInGameStatus();

bool Account_CanMakeTransaction(uint32_t player_id, float amount);
void Account_AddMoney(uint32_t player_id, float amount);
void Account_SubtractMoney(uint32_t player_id, float amount);
void Account_SetMoney(uint32_t player_id, float amount);
float Account_GetMoney(uint32_t player_id);

char *Account_GetSaveName();
char *Account_GetPlayerName(uint32_t player_id);
Vector *Account_GetPlayers();

#endif