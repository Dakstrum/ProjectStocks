
#include "dbcard.h"
#include "dbsave.h"
#include "shared.h"
#include "account.h"
#include "cardcontroller.h"
#include "simulation_card.h"

#include "vector.h"

static const float CARD_CHANCE = 4.0/(24.0 * 30.0);

void simulation_card_init()
{


    
}

void simulation_card_add_random(uint32_t player_id)
{

	Vector *cards     = dbcard_get_all_cards();
	Card *cards_temp  = cards->elements;

	uint32_t card_idx = rand () % cards->num_elements;
	


}

uint32_t simulation_card_pick_player()
{

    return 0;

}

void simulation_card_step(uint16_t seed[3])
{

    Vector *players = Account_GetPlayers();
    Player *players_temp = players->elements;
    for (size_t i = 0; i < players->num_elements;i++) {

    	if (shared_random_float() <= CARD_CHANCE) {

    		simulation_card_add_random(players_temp[i].id);

    	}

    }

}