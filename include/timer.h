#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>

void Timer_Init();
double Timer_GetDiff();
void Timer_Pause();
void Timer_Unpause();
bool Timer_IsPaused();

#endif