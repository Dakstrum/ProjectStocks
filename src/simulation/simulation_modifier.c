#include <stdio.h>

#include "dbcard.h"
#include "dbcompany.h"
#include "dbaccount.h"

#include "account.h"

#include "vector.h"

#include "simulation_modifier.h"
#include "simulation_event.h"

static Vector *modifiers = NULL;
static Vector *companies = NULL;

void simulation_modify_company(uint32_t company_id, time_t play_time, float modifier, uint32_t days, char *event)
{

    PlayedCard temp = {company_id, play_time, modifier, days};
    Vector_PushBack(modifiers, &temp);
    Simulation_Event_Push(event, play_time);

}
 
void simulation_modify_category(uint32_t category_id, time_t play_time, float modifier, uint32_t days, char *event)
{

    Company *companies_temp = companies->elements;
    for (size_t i = 0; i < companies->num_elements;i++) {

        if (companies_temp[i].category_id == category_id) {

            PlayedCard temp = {companies_temp[i].company_id, play_time, modifier, days};
            Vector_PushBack(modifiers, &temp);

        }

    }
    Simulation_Event_Push(event, play_time);

}


void simulation_modify_global(float modifier, time_t play_time, uint32_t days, char *event)
{

    Company *companies_temp = companies->elements;
    for (size_t i = 0; i < companies->num_elements;i++) {

        PlayedCard temp = {companies_temp[i].company_id, play_time, modifier, days};
        Vector_PushBack(modifiers, &temp);

    }
    Simulation_Event_Push(event, play_time);

}

void simulation_apply_transaction(int transaction_amount, uint32_t company_id, time_t transaction_time)
{

    float modifier = transaction_amount/10000.0;
    simulation_modify_company(company_id, transaction_time, modifier, 7, NULL);

}

void simulation_modifiers_init(Vector *current_companies) 
{

    if (modifiers != NULL) {

        Vector_Reset(modifiers);

    } else {

        modifiers = Vector_Create(sizeof(SimulationModifier), 32);

    }

	companies = current_companies;
    Vector *played_cards = dbcard_get_played_cards();
    Vector *transactions = dbaccount_get_all_transactions();

    Transaction *temp = transactions->elements;
    for (size_t i = 0; i < transactions->num_elements;i++)
        simulation_apply_transaction(temp[i].shares_exchanged, temp[i].company_id, temp[i].transaction_date);

    char buffer[128];
    PlayedCard *played_cards_temp = played_cards->elements;
    for (size_t i = 0; i < played_cards->num_elements;i++) {

        sprintf(buffer, "%s played card %s against company %s", Account_GetPlayerName(played_cards_temp[i].player_id), GetCardTitle(played_cards_temp[i].card_id), GetCompanyNameRef(played_cards_temp[i].company_id));
        buffer[127] = '\0';
        simulation_modify_company(played_cards_temp[i].company_id, played_cards_temp[i].played_time, played_cards_temp[i].price_modifier, played_cards_temp[i].modifier_length, buffer);

    }

}

void simuation_remove_old_modifiers(time_t t)
{

    PlayedCard *modifiers_temp = modifiers->elements;
    for (size_t i = 0; i < modifiers->num_elements;i++) {

        if (t < modifiers_temp[i].played_time)
            continue;

        Vector_Remove(modifiers, i);
        i--;

    }

}


Vector *simulation_get_modifiers()
{

	return modifiers;

}