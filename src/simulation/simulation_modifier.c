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

void simulation_apply_card(uint32_t player_id, uint32_t card_id, uint32_t company_id, time_t play_time)
{

    float price_modifier  = GetCardPriceModifier(card_id);
    float modifier_length = GetCardModifierLength(card_id);

    char buffer[128];
    sprintf(buffer, "%s played card %s against company %s", Account_GetPlayerName(player_id), GetCardTitle(card_id), GetCompanyNameRef(company_id));
    buffer[127] = '\0';
    simulation_modify_company(company_id, play_time, price_modifier, modifier_length, buffer);

}

void simulation_modifiers_init(Vector *current_companies) 
{

    if (modifiers != NULL){

        Vector_Reset(modifiers);

    } else {

        modifiers = Vector_Create(sizeof(SimulationModifier), 32);
        
    }

	companies = current_companies;
    Vector *played_cards = dbcard_get_played_cards();
    Vector *transactions = dbaccount_get_all_transactions();

    Vector_ForEach(idx, element, transactions, Transaction *)
        simulation_apply_transaction(element->shares_exchanged, element->company_id, element->transaction_date);

    Vector_ForEach(idx, element, played_cards, PlayedCard *)
        simulation_apply_card(element->player_id, element->card_id, element->company_id, element->played_time);

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

void simulation_reset_modifiers()
{

    if (modifiers != NULL) {

        Vector_Delete(modifiers);
        modifiers = NULL;

    }

}