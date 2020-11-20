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

static const int HOURS_IN_YEAR         = 8760;
static const int HOURS_IN_QUARTER_YEAR = HOURS_IN_YEAR / 4;
static const int HOURS_IN_HALF_YEAR    = HOURS_IN_YEAR / 2;
static const int HOURS_IN_TWO_YEARS    = HOURS_IN_YEAR * 2;

static const float TRUNCATE_TO_AMOUNT = 500.0;
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

void ReduceStockPriceAmount(StockPrices *prices)
{

    Log("ReduceStockPriceAmount: STUB Remove me!");

}

float GetRandomFloat()
{

    return (float)(rand()/RAND_MAX);

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

Vector *GetStockPricesFromNowUntil(char *company_name, time_t span)
{

/*
    int company_idx = GetCompanySimIndex(company_name);
    if (company_idx == -1)
        return NULL;

    time_t current_time  = Account_GetGameTime();
    time_t previous_time = current_time - span;

    if (span == 0)
        previous_time = 0;

    if (previous_time < 0)
        previous_time = 0;

    StockPrices *prices = malloc(sizeof(StockPrices));
    InitializeStockPrice(prices);
    for (size_t i = 0; i < sim_data.prices[company_idx].num_prices;i++) {

        if (sim_data.prices[company_idx].times[i] > current_time)
            break;

        if (sim_data.prices[company_idx].times[i] >= previous_time && sim_data.prices[company_idx].times[i] <= current_time)
            StoreStockPrice(prices,sim_data.prices[company_idx].prices[i], sim_data.prices[company_idx].times[i]);

    }
    ReclaimUnusedStockPriceMemory(prices);

    return prices;
    */
    return NULL;

}

float CurrentStockPrice(char *company_name) 
{

    /*
    int company_idx = GetCompanySimIndex(company_name);
    if (company_idx == -1)
        return -1.0f;

    time_t current_time = Account_GetGameTime(); 
    for (size_t i = 0; i < sim_data.prices[company_idx].num_prices; i++)
        if (sim_data.prices[company_idx].times[i] == current_time)
            return sim_data.prices[company_idx].prices[i];

    */

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
    for (size_t i = 0; i < companies->num_elements;i++) {

        sim_data[i] = Vector_Create(sizeof(StockPrice), 4096);

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