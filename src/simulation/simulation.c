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

}

float GetRandomSign()
{

    return shared_nrand48(current_seed) % 2 == 0 ? -1.0 : 1.0;

}

float GetRandomFloat()
{

    return (float)shared_nrand48(current_seed)/2147483648;

}

float GetRandomEventMagnitude()
{

    float random = GetRandomFloat();

    if (random >= .2)
        return .25;
    else if (random >= 0.05)
        return .5;
    else
        return .75;

}

float GenerateRandomPriceFluctuation(float last_price) 
{

    return GetRandomSign() * GetRandomFloat() * last_price * .0075;

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

Vector *GetStockPricesBetween(Vector *sim_prices, time_t start_time, time_t end_time)
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
    return GetStockPricesBetween(sim_data[company_idx], previous_time, current_time);

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


void Simulation_SimulateStep(time_t t)
{

    float value = 0.0;
    StockPrice *prices = NULL;
    for (size_t i = 0; i < companies->num_elements;i++) {

        prices = sim_data[i]->elements;
        value  = prices[sim_data[i]->num_elements-1].price;
        value  = value + GenerateRandomPriceFluctuation(value);

        StockPrice price = {value, t};
        Vector_PushBack(sim_data[i], &price);

    }

}

void Simulation_SimulateUntil(time_t t)
{

    time_t temp = 0;
    while (temp < t) {

        Simulation_SimulateStep(temp);
        temp += HOUR;

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