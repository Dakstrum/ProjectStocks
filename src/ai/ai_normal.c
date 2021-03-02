
#include "ai_normal.h"

#include "dbsave.h"
#include "dbcard.h"
#include "shared.h"
#include "dbcompany.h"
#include "account.h"
#include "portfolio.h"
#include "transaction.h"
#include "vector.h"

bool ai_normal_does_other_own_at_least_percent(uint32_t player_id, uint32_t company_id, float percent)
{

    Vector *players = Account_GetPlayers();
    Player *players_temp = players->elements;

    for (size_t i = 0; i < players->num_elements;i++) {

        if (players_temp[i].player_id == player_id)
            continue;

        if (portfolio_get_percentage(player_id, company_id) >= percent)
            return true;

    }

    return false;

}

void ai_normal_attempt_negative_card_play(uint32_t player_id, uint32_t card_id)
{

    Vector *companies = GetAllCompaniesVector();
    Company *companies_temp = companies->elements;

    for (size_t i = 0; i < companies->num_elements;i++) {

        float owned_percentage = portfolio_get_percentage(player_id, companies_temp[i].company_id);
        bool ai_owns_30_perc   = ai_normal_does_other_own_at_least_percent(player_id, companies_temp[i].company_id, 30.0f);

    }

}

void ai_normal_attempt_positive_card_play(uint32_t player_id, uint32_t card_id)
{

    Vector *companies = GetAllCompaniesVector();
    Company *companies_temp = companies->elements;

    for (size_t i = 0; i < companies->num_elements;i++) {

        float owned_percentage = portfolio_get_percentage(player_id, companies_temp[i].company_id);
        bool ai_owns_70_perc   = ai_normal_does_other_own_at_least_percent(player_id, companies_temp[i].company_id, 70.0f);
        float random_chance    = shared_random_float();
        float chance           = owned_percentage/30.0f - 0.4f - ai_owns_70_perc == true ? 1.0f : 0.0f;

        if (chance > random_chance) {

            dbcard_apply_card(player_id, card_id, companies_temp[i].company_id);
            break;

        }

    }

}

void ai_normal_play_cards(uint32_t player_id, time_t t) 
{

    Vector *cards = dbcard_get_player_cards(player_id);
    if (cards->num_elements == 0)
        return;

    PlayerCard *cards_temp = cards->elements;
    for (size_t i = 0; i < cards->num_elements;i++) {

        if (GetCardPriceModifier(cards_temp[i].card_id) < 0)
            ai_normal_attempt_negative_card_play(player_id, cards_temp[i].card_id);
        else
            ai_normal_attempt_positive_card_play(player_id, cards_temp[i].card_id);

    }

    Vector_Delete(cards);

}

void ai_normal_conduct_transactions(uint32_t player_id, time_t t)
{


}