
#include "dbcard.h"
#include "dbsave.h"
#include "shared.h"
#include "account.h"
#include "cardcontroller.h"
#include "simulation_card.h"
#include "menupersistence.h"

#include "vector.h"

static const float CARD_CHANCE = 0.5/(24.0 * 30.0);

void simulation_card_init()
{


    
}

void simulation_card_add_random(uint32_t player_id)
{

	Vector *cards     = dbcard_get_all_cards();
	Card *cards_temp  = cards->elements;

	Vector *player_cards = dbcard_get_player_cards(player_id);

	if (player_cards->num_elements == 5)
		return;

	uint32_t card_idx = rand () % cards->num_elements;
	dbcard_add_card_to_player(player_id, cards_temp[card_idx].card_id);

	if (persistence_get_current_menu() == STOCKS && player_id == Account_GetPlayerId())
		CardController_InitCard();

}

uint32_t simulation_card_pick_player()
{

    return 0;

}

void simulation_card_step()
{

    Vector *players = Account_GetPlayers();
    Player *players_temp = players->elements;
    for (size_t i = 0; i < players->num_elements;i++) {

    	if (shared_random_float() <= CARD_CHANCE) {

    		simulation_card_add_random(players_temp[i].id);

    	}

    }

}