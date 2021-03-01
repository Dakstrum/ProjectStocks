#ifndef SIMULATION_AI_H
#define SIMULATION_AI_H

#include <stdlib.h>

typedef enum DifficultyLevel {

	EASY,
	NORMAL,
	HARD

} DifficultyLevel;

void simulation_ai_step(time_t t);
void simulation_ai_init(DifficultyLevel level);

#endif