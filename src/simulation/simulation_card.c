
#include "dbcard.h"
#include "dbsave.h"
#include "account.h"
#include "cardcontroller.h"
#include "simulation_card.h"

#include "vector.h"

static const float CARD_CHANCE = 4.0/(24.0 * 30.0);

void simulation_card_init()
{


    
}

void simulation_card_add(uint32_t player_id)
{


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



    }

}