
#include "ai_normal.h"

#include "dbsave.h"
#include "dbcard.h"
#include "dbcompany.h"
#include "dbaccount.h"

#include "account.h"
#include "portfolio.h"
#include "transaction.h"

#include "company_utils.h"

#include "log.h"
#include "vector.h"
#include "shared.h"

#include "simulation_modifier.h"

bool ai_normal_does_other_own_at_least_percent(uint32_t player_id, uint32_t company_id, float percent)
{

    Vector *players = Account_GetPlayers();
    Player *players_temp = players->elements;

    for (size_t i = 0; i < players->num_elements;i++) {

        if (players_temp[i].player_id == player_id)
            continue;

        if (portfolio_get_percentage(players_temp[i].player_id, company_id) >= percent)
            return true;

    }

    return false;

}

void ai_normal_attempt_negative_card_play(uint32_t player_id, uint32_t card_id, time_t t)
{

    if (shared_random_float() >= 0.75f)
        return;

    Vector *companies = company_utils_get_all_active();

    Vector_ForEach(i, company, companies, Company *) {

        float owned_percentage = portfolio_get_percentage(player_id, company->company_id);
        bool ai_owns_30_perc   = ai_normal_does_other_own_at_least_percent(player_id, company->company_id, 0.30f);

        float chance = (ai_owns_30_perc == true ? 1.0f : 0.0f) - owned_percentage/0.10f;
        if (chance >= shared_random_float()) {

            simulation_apply_card(player_id, card_id, company->company_id, t);
            dbcard_apply_card(player_id, card_id, company->company_id);
            break;

        }

    }
    Vector_Delete(companies);

}

void ai_normal_attempt_positive_card_play(uint32_t player_id, uint32_t card_id, time_t t)
{

    if (shared_random_float() >= 0.75f)
        return;

    Vector *companies = company_utils_get_all_active();

    Vector_ForEach(i, company, companies, Company *) {

        float owned_percentage = portfolio_get_percentage(player_id, company->company_id);
        bool ai_owns_70_perc   = ai_normal_does_other_own_at_least_percent(player_id, company->company_id, 0.70f);
        float chance           = owned_percentage/0.30f - ai_owns_70_perc == true ? 1.0f : 0.0f;

        if (chance >= shared_random_float()) {

            simulation_apply_card(player_id, card_id, company->company_id, t);
            dbcard_apply_card(player_id, card_id, company->company_id);
            break;

        }

    }
    Vector_Delete(companies);

}

void ai_normal_play_cards(uint32_t player_id, time_t t) 
{

    Vector *cards = dbcard_get_player_cards(player_id);
    if (cards->num_elements == 0)
        return;

    PlayerCard *cards_temp = cards->elements;
    for (size_t i = 0; i < cards->num_elements;i++) {

        if (dbcard_get_card_modifier(cards_temp[i].card_id) < 0)
            ai_normal_attempt_negative_card_play(player_id, cards_temp[i].card_id, t);
        else
            ai_normal_attempt_positive_card_play(player_id, cards_temp[i].card_id, t);

    }

    Vector_Delete(cards);

}

void ai_buy_shares(uint32_t player_id, time_t t)
{

    if (shared_random_float() >= 0.5f)
        return;

    float money = Account_GetMoney(player_id);
    if (money == 0.0f)
        return;

    Vector *random_companies = company_utils_get_random(3);
    Vector *lowest_companies = company_utils_get_lowest(2);
    Vector *companies = Vector_Concat(random_companies, lowest_companies);

    Vector_ForEach(i, company, companies, Company *) {

        if (shared_random_float() >= 0.5f)
            continue;

        float quarter_money     = money * 0.25;
        float others_own_chance = ai_normal_does_other_own_at_least_percent(player_id, company->company_id, 0.05f) == true ? 0.25f : 0.0f;
        float random_chance     = shared_random_float() - 0.1;
        float modifier_chance   = simulation_negative_modifiers_active(company->company_id, t) == true ? 1.0f : 0.0f;

        if (others_own_chance + random_chance + modifier_chance >= shared_random_float())
            transaction_purchase_stock(player_id, company->company_id, quarter_money);

        money = Account_GetMoney(player_id);
        if (money == 0.0f)
            break;

    }

    Vector_Delete(random_companies);
    Vector_Delete(lowest_companies);
    Vector_Delete(companies);

}

void ai_sell_shares(uint32_t player_id, time_t t)
{

    if (shared_random_float() >= 0.5f)
        return;

    Vector *companies = company_utils_get_all_active();

    Vector_ForEach(i, company, companies, Company *) {

        dbaccount_get_owned_stock_amount(player_id, company->company_id);

        float modifier_positive_chance = simulation_positive_modifiers_active(company->company_id, t) == true ? -2.0f : 0.0f;
        float random_chance = shared_random_float() - 0.1;

        if (random_chance + modifier_positive_chance >= shared_random_float()) {



        }

    }

    Vector_Delete(companies);

}

void ai_normal_conduct_transactions(uint32_t player_id, time_t t)
{

    ai_buy_shares(player_id, t);
    //ai_sell_shares(player_id, t);

}