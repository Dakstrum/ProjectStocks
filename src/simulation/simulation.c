#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>

#include <sqlite3.h>
#include <allegro5/allegro.h>

#include "log.h"
#include "vector.h"
#include "shared.h"
#include "account.h"
#include "dbcard.h"
#include "dbaccess.h"
#include "dbevents.h"
#include "dbcompany.h"
#include "simulation.h"
#include "simulation_event.h"

static const int HOUR = 3600;
static const int SIX_HOURS = HOUR * 6;
static int end_year   = 0;

static atomic_bool simulation_finished;

static Vector *companies;
static Vector **sim_data;
static Vector *modifiers;

static uint16_t current_seed[3];

void CleanupBeforeExit();

void SimulationLoop(unsigned int idx);
void GenerateDataForCompanies();
void IncrementCurrentTimeByHour();
bool ShouldContinueSimulation(time_t current_time);

void SetYearLapse(int years_to_lapse);
int GetYearFromBuff(char *buff);

bool IsSimulationDone() 
{

    return atomic_load(&simulation_finished);

}

uint32_t Simulation_CompanyIndex(char *company_name)
{

    Company *companies_temp = companies->elements;
    for (size_t i = 0; i < companies->num_elements;i++) {

        if (strcmp(company_name, companies_temp[i].company_name) == 0) {

            return i;

        }

    }
    assert(0 == 1);
    return 0;

}

float GenerateRandomPriceFluctuation(float last_price) 
{

    return shared_get_random_sign(current_seed) * shared_get_random_float(current_seed) * last_price * .0075;

}

void SetYearLapse(int years_to_lapse) 
{

    years_to_lapse = years_to_lapse + 1; // OFFSET by 1 year to fake the approximate correct amount of time.
    
    char buff[128];
    time_t start_time = 0;

    strftime(buff, sizeof(buff), "%Y", localtime(&start_time));
    end_year = GetYearFromBuff(buff) + years_to_lapse;


}

bool ShouldContinueSimulation(time_t current_time) 
{

    char current_time_buff[128];
    strftime(current_time_buff, sizeof(current_time_buff), "%Y", localtime(&current_time));

    return GetYearFromBuff(current_time_buff) < end_year;

}

int GetYearFromBuff(char *buff) 
{

    char year_buff[5];
    sscanf(buff, "%s", year_buff);
    year_buff[4] = '\0';
    return atoi(year_buff);

}

Vector *Simulation_GetStockPricesBetween(Vector *sim_prices, time_t start_time, time_t end_time)
{

    Vector *prices = Vector_Create(sizeof(StockPrice), 128);
    StockPrice *temp_prices = sim_prices->elements;

    for (size_t i = 0; i < sim_prices->num_elements;i++) {

        if (temp_prices[i].date >= start_time && temp_prices[i].date <= end_time)
            Vector_PushBack(prices, &temp_prices[i]);

    }

    return prices;


}

Vector *Simulation_GetStockPrices(char *company_name, time_t current_time, time_t span)
{

    time_t previous_time = current_time - span;

    if (span > current_time)
        previous_time = 0;

    if (span == 0)
        previous_time = 0;

    uint32_t company_idx = Simulation_CompanyIndex(company_name);
    return Simulation_GetStockPricesBetween(sim_data[company_idx], previous_time, current_time);

}

float Simulation_GetLastStockPrice(char *company_name) 
{

    uint32_t company_idx = Simulation_CompanyIndex(company_name);
    StockPrice *prices   = sim_data[company_idx]->elements;
    return prices[sim_data[company_idx]->num_elements - 1].price;

}

char *GetAnyEventAtTime(time_t event_time)
{

    return NULL;

}

float Simulation_GetStockPriceDiff(char *company_name)
{

    uint32_t company_idx  = Simulation_CompanyIndex(company_name);
    uint32_t num_elements = sim_data[company_idx]->num_elements;

    if (num_elements < 2)
        return 0.0;

    StockPrice *prices = sim_data[company_idx]->elements;
    return prices[num_elements - 1].price - prices[num_elements - 2].price;

}

void Simulation_ModifyCompany(uint32_t company_id, time_t play_time, float modifier, uint32_t days, char *event)
{

    PlayedModifiers temp = {company_id, play_time, modifier, days};
    Vector_PushBack(modifiers, &temp);
    Simulation_Event_Push(event, play_time);

}
 
void Simulation_ModifyCategory(uint32_t category_id, time_t play_time, float modifier, uint32_t days, char *event)
{

    Company *companies_temp = companies->elements;
    for (size_t i = 0; i < companies->num_elements;i++) {

        if (companies_temp[i].category_id == category_id) {

            PlayedModifiers temp = {companies_temp[i].company_id, play_time, modifier, days};
            Vector_PushBack(modifiers, &temp);

        }

    }
    Simulation_Event_Push(event, play_time);

}

void Simulation_ModifyGlobal(float modifier, time_t play_time, uint32_t days, char *event)
{

    Company *companies_temp = companies->elements;
    for (size_t i = 0; i < companies->num_elements;i++) {

        PlayedModifiers temp = {companies_temp[i].company_id, play_time, modifier, days};
        Vector_PushBack(modifiers, &temp);

    }
    Simulation_Event_Push(event, play_time);

}

float Simulation_GetNextValue(time_t t, size_t idx) 
{

    StockPrice *prices = sim_data[idx]->elements;
    float value = prices[sim_data[idx]->num_elements-1].price;
    float price_fluctuation = GenerateRandomPriceFluctuation(value);

    PlayedModifiers *modifiers_temp = modifiers->elements;
    Company *companies_temp = companies->elements;
    for (size_t i = 0; i < modifiers->num_elements;i++) {

        if (companies_temp[idx].company_id != modifiers_temp[i].company_id)
            continue;

        if (t < modifiers_temp[i].played_time || t > modifiers_temp[i].played_time + modifiers_temp[i].modifier_length * 86400)
            continue;
        
        value = value + value * modifiers_temp[i].price_modifier / ((float)modifiers_temp[i].modifier_length * 4.0f);

    }

    return value + price_fluctuation;

}

void Simulation_EventStep(time_t t)
{

    if (!Simulation_Event_EventChanceCheck(current_seed))
        return;

    Event *event = Simulation_Event_GetRandomEvent(current_seed);
    switch (event->event_type) {

        case GLOBAL:
            Simulation_ModifyGlobal(event->price_modifier, t, event->modifier_length, event->event);
            break;
        case CATEGORY: 
            Simulation_ModifyCategory(event->id, t, event->price_modifier, event->modifier_length, event->event);
            break;
        case COMPANY:
            Simulation_ModifyCompany(event->id, t, event->price_modifier, event->modifier_length, event->event);
            break;

    }

}

void Simulation_CardStep(time_t t) 
{

    

}

void Simulation_RemoveOldModifiers(time_t t)
{

    PlayedModifiers *modifiers_temp = modifiers->elements;
    for (size_t i = 0; i < modifiers->num_elements;i++) {

        if (t < modifiers_temp[i].played_time)
            continue;

        Vector_Remove(modifiers, i);
        i--;

    }

}

void Simulation_SimulateStep(time_t t)
{

    Simulation_EventStep(t);
    Simulation_CardStep(t);
    for (size_t i = 0; i < companies->num_elements;i++) {

        StockPrice price = {Simulation_GetNextValue(t, i), t};
        Vector_PushBack(sim_data[i], &price);

    }
    Simulation_RemoveOldModifiers(t);

}

void Simulation_SimulateUntil(time_t t)
{

    time_t temp = 0;
    while (temp < t) {

        Simulation_SimulateStep(temp);
        temp += SIX_HOURS;

    }

    atomic_store(&simulation_finished, true);

}

void Simulation_LoadModifiers() 
{

    modifiers = DBCards_GetPlayedModifiersCopy();

}

void Simulation_LoadCompanies()
{

    assert(sim_data == NULL);

    companies = GetAllCompaniesVector();
    sim_data  = malloc(sizeof(Vector *) * companies->num_elements);
    Company *companies_temp = companies->elements;

    for (size_t i = 0; i < companies->num_elements;i++) {

        StockPrice stock_price = {companies_temp[i].ipo, 0};
        sim_data[i] = Vector_Create(sizeof(StockPrice), 4096);
        Vector_PushBack(sim_data[i], &stock_price);

    }

}

void Simulation_InitRandom(uint32_t new_game_seed)
{
    srand(new_game_seed);
    current_seed[0] = (new_game_seed >> 16); 
    current_seed[1] = (new_game_seed ^ 0xFFFF0000);
    current_seed[2] = (current_seed[0] ^ current_seed[1]);

}

void Simulation_Init(uint32_t new_game_seed)
{

    Simulation_InitRandom(new_game_seed);
    Simulation_Event_Init();
    Simulation_LoadCompanies();
    Simulation_LoadModifiers();

}

void Simulation_Reset()
{


    atomic_store(&simulation_finished, false);
    Vector_Delete(modifiers);

    for (size_t i = 0; i < companies->num_elements;i++) {

        Vector_Delete(sim_data[i]);

    }

    free(sim_data);

    sim_data  = NULL;
    modifiers = NULL;
    companies = NULL;

}

void StopSimulation()
{

    atomic_store(&simulation_finished, false);
    //CleanSimulation();

}