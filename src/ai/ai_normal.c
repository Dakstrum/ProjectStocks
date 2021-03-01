
#include "ai_normal.h"

#include "dbsave.h"
#include "dbcard.h"
#include "dbcompany.h"
#include "account.h"
#include "portfolio.h"
#include "transaction.h"
#include "vector.h"

/*

typedef struct PlayerCard {

    uint32_t player_card_id;
    uint32_t player_id;
    uint32_t card_id;

} PlayerCard;

void dbcard_apply_card(uint32_t player_id, uint32_t card_id, uint32_t company_id);

typedef struct Company {

    unsigned int company_id;
    unsigned int category_id;
    float ipo;
    char company_name[64];
    char company_description[256];
    char company_abbreviation[6];

} Company;

typedef struct Player {

    uint32_t player_id;
    float money;
    char name[32];
    bool save_owner;

} Player;

*/

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

        float chance = 0.0f;
        float owned_percentage = portfolio_get_percentage(player_id, companies_temp[i].company_id);
        bool ai_owns_70_perc   = ai_normal_does_other_own_at_least_percent(player_id, companies_temp[i].company_id, 70.0f);

    }

}

void ai_normal_attempt_positive_card_play(uint32_t player_id, uint32_t card_id)
{

    Vector *companies = GetAllCompaniesVector();
    Company *companies_temp = companies->elements;

    for (size_t i = 0; i < companies->num_elements;i++) {



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