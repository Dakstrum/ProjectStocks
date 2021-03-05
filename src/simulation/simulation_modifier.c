#include <stdio.h>

#include "dbcard.h"
#include "dbcompany.h"
#include "dbaccount.h"

#include "account.h"

#include "log.h"
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

    float modifier = transaction_amount/100000.0;
    simulation_modify_company(company_id, transaction_time, modifier, 7, NULL);

}

void simulation_apply_card(uint32_t player_id, uint32_t card_id, uint32_t company_id, time_t play_time)
{

    float price_modifier     = dbcard_get_card_modifier(card_id);
    uint32_t modifier_length = dbcard_get_card_modifier_length(card_id);

    char buffer[128];
    sprintf(buffer, "%s played card %s against company %s", Account_GetPlayerName(player_id), dbcard_get_card_name(card_id), GetCompanyNameRef(company_id));
    buffer[127] = '\0';
    simulation_modify_company(company_id, play_time, price_modifier, modifier_length, buffer);

}

time_t simulation_modifier_get_endtime(SimulationModifier *modifier)
{

    return modifier->played_time + modifier->modifier_length * 86400;

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

    SimulationModifier *modifiers_temp = modifiers->elements;
    for (size_t i = 0; i < modifiers->num_elements;i++) {

        if (t < simulation_modifier_get_endtime(&modifiers_temp[i]))
            continue;

        Vector_Remove(modifiers, i);
        i--;

    }

}


Vector *simulation_get_modifiers()
{

	return modifiers;

}

bool simulation_negative_modifiers_active(uint32_t company_id, time_t t)
{

    Vector_ForEach(i, modifier, modifiers, SimulationModifier *) {

        if (modifier->company_id != company_id)
            continue;
        if (modifier->price_modifier > 0.0f)
            continue;
        if (modifier->played_time >= t && t <= simulation_modifier_get_endtime(modifier))
            return true;
    }
    return false;

}

bool simulation_positive_modifiers_active(uint32_t company_id, time_t t)
{

    Vector_ForEach(i, modifier, modifiers, SimulationModifier *) {

        if (modifier->company_id != company_id)
            continue;
        if (modifier->price_modifier < 0.0f)
            continue;
        if (modifier->played_time >= t && t <= simulation_modifier_get_endtime(modifier))
            return true;

    }
    return false;
    
}

void simulation_reset_modifiers()
{

    if (modifiers != NULL) {

        Vector_Delete(modifiers);
        modifiers = NULL;

    }

}