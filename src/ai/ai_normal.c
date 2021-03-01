
#include "ai_normal.h"

#include "dbsave.h"
#include "dbcard.h"
#include "account.h"
#include "transaction.h"
#include "vector.h"

void ai_normal_play_cards(uint32_t player_id, time_t t) 
{

	Vector *cards = dbcard_get_player_cards(player_id);

}

void ai_normal_conduct_transactions(uint32_t player_id, time_t t)
{


}