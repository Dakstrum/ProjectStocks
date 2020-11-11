#ifndef GAME_H
#define GAME_H

void Game_Init();
void Game_Reset();

char *Game_GetDate();

time_t Game_GetGameTime();
void Game_SetGameTime(time_t new_game_time);

void Game_SetSpeed(const int speed);


#endif