
#include <stdint.h>

#include "dbsave.h"
#include "dbcard.h"
#include "account.h"
#include "transaction.h"
#include "simulation_ai.h"

#include "vector.h"

void simulation_ai_play_cards(uint32_t player_id, time_t t)
{

	Vector *cards = dbcard_get_player_cards(player_id);

}

void simulation_ai_conduct_transactions(uint32_t player_id, time_t t)
{


}

void simulation_ai_step(time_t t)
{

	Vector *players = Account_GetPlayers();
	Player *players_temp = players->elements;
	for (size_t i = 0; i < players->num_elements;i++) {

		if (players_temp[i].save_owner)
			continue;

		simulation_ai_play_cards(players_temp[i].player_id, t);
		simulation_ai_conduct_transactions(players_temp[i].player_id, t);

	}

}