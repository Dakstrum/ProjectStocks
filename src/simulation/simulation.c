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

static const int HOUR = 3600;
static int end_year   = 0;

static atomic_bool simulation_finished;

static Vector *companies;
static Vector **sim_data;
static Vector *modifiers;

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

void SimulateToSavePoint()
{

    atomic_store(&simulation_finished, true);

}

void StoreStockPrice(Vector *prices, float price, time_t timestamp)
{

    StockPrice temp = {price, timestamp};
    Vector_PushBack(prices, &temp);

}

float GetRandomSign()
{

    return rand() % 2 == 0 ? -1.0 : 1.0;

}

float GetRandomFloat()
{

    return (float)rand()/(float)RAND_MAX;

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

Vector *GetStockPricesFromNowUntil(char *company_name, time_t current_time, time_t span)
{

    Company *companies_temp = companies->elements;
    time_t previous_time = current_time - span;

    if (span > current_time)
        previous_time = 0;

    if (span == 0)
        previous_time = 0;

    for (size_t i = 0; i < companies->num_elements;i++) {

        if (strcmp(company_name, companies_temp[i].company_name) != 0)
            continue;

        return GetStockPricesBetween(sim_data[i], previous_time, current_time);

    }

    return NULL;

}

float CurrentStockPrice(char *company_name) 
{

    Company *companies_temp = companies->elements;
    StockPrice *prices = NULL;
    for (size_t i = 0; i < companies->num_elements;i++) {

        if (strcmp(company_name, companies_temp[i].company_name) == 0) {

            prices = sim_data[i]->elements;
            return prices[sim_data[i]->num_elements - 1].price;

        }

    }

    return -1.0f;

}

char *GetAnyEventAtTime(time_t event_time)
{

    return NULL;

}

float GetCurrentStockChange(char *company_name)
{
    static float last_price    = 0.0;
    static float current_price = 0.0;
    static float change        = 0.0;

    if(current_price != CurrentStockPrice(company_name))
        last_price = current_price;

    current_price = CurrentStockPrice(company_name);
    change        = current_price - last_price;
    
    return change;
}

void Simulation_ModifyCompany(uint32_t company_id, float modifier, uint32_t days, char *event)
{



}
 
void Simulation_ModifyCategory(uint32_t category_id, float modifier, uint32_t days, char *event)
{



}

void Simulation_ModifyGlobal(float modifier, uint32_t days, char *event)
{



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

    time_t temp = t;
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

void Simulation_Init(uint32_t new_game_seed)
{

    srand(new_game_seed);
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