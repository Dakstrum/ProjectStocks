#ifndef GAME_H
#define GAME_H

void Game_Init();
void Game_Reset();

char *Game_GetDate();

time_t Game_GetGameTime();
void Game_SetGameTime(time_t new_game_time);

void Game_SetSpeed(const int speed);

void Game_SetSeed(uint32_t new_seed);
void Game_SetSaveId(uint32_t new_save_id);

uint32_t Game_GetSeed();
uint32_t Game_GetSaveId();


#endif