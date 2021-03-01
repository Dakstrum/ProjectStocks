
#include <stdint.h>

#include "simulation_ai.h"

#include "account.h"
#include "dbsave.h"

#include "ai_easy.h"
#include "ai_normal.h"
#include "ai_hard.h"

void (*ai_play_cards)(uint32_t player_id, time_t t)           = ai_normal_play_cards;
void (*ai_conduct_transactions)(uint32_t player_id, time_t t) = ai_normal_conduct_transactions;

void simulation_ai_step(time_t t)
{

	Vector *players = Account_GetPlayers();
	Player *players_temp = players->elements;
	for (size_t i = 0; i < players->num_elements;i++) {

		if (players_temp[i].save_owner)
			continue;

		ai_play_cards(players_temp[i].player_id, t);
		ai_conduct_transactions(players_temp[i].player_id, t);

	}

}

void simulation_ai_init(DifficultyLevel level)
{

	switch (level) {

		case EASY:
			ai_play_cards = ai_easy_play_cards;
			ai_conduct_transactions = ai_easy_conduct_transactions;
			break;
		case NORMAL:
			ai_play_cards = ai_normal_play_cards;
			ai_conduct_transactions = ai_normal_conduct_transactions;
			break;
		case HARD:
			ai_play_cards = ai_hard_play_cards;
			ai_conduct_transactions = ai_hard_conduct_transactions;
			break;

	}

}