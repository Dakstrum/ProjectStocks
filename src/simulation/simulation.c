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

#define STOCK_PRICE_SIZE 4096

typedef struct Sim 
{

    Vector *companies;
    Vector **prices;
    Vector *random_event_chance;

    unsigned int num_companies;

} Sim;

typedef struct EventSim 
{

    Vector *event_times;
    Vector *events;

} EventSim;

typedef struct EventSimId 
{

    Vector *ids;
    Vector *event_sims;

} EventSimId;

typedef struct SimulationFrame
{

    float last_price;
    float price;
    time_t current_time;
    Event *event;

    time_t event_end_time;
    float event_price_diff;

} SimulationFrame;

static const int HOURS_IN_YEAR         = 8760;
static const int HOURS_IN_QUARTER_YEAR = HOURS_IN_YEAR / 4;
static const int HOURS_IN_HALF_YEAR    = HOURS_IN_YEAR / 2;
static const int HOURS_IN_TWO_YEARS    = HOURS_IN_YEAR * 2;

static const float TRUNCATE_TO_AMOUNT = 500.0;
static const int HOUR = 3600;
static int end_year   = 0;

static atomic_bool simulation_finished;

static Sim sim_data;
static int save_id   = 0;
static uint32_t seed = 0;
static time_t step_time = 0;

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

void InitializeSimulation() 
{

    atomic_store(&simulation_finished, false);

}

void SetRandomSeed() 
{

    /*

    if (save_id != -1)
        seed = Account_GetSaveSeed();
    else
        seed = time(NULL);

    */

}

void SetSaveId()
{

    /*
    save_id = Account_GetSaveId();

    */

}

void SetCompanies() 
{

    sim_data.companies = GetAllCompaniesVector();
    sim_data.prices    = malloc(sizeof(Vector *) * sim_data.companies->num_elements);

    for (size_t i = 0; i < sim_data.companies->num_elements;i++)
        sim_data.prices[i] = Vector_Create(sizeof(StockPrice), STOCK_PRICE_SIZE);

}

void SimulateToSavePoint()
{

    atomic_store(&simulation_finished, true);

}

void *StockSimulationEntry(ALLEGRO_THREAD *thread, void *arg) 
{

    SetYearLapse(25);
    SetSaveId();
    SetRandomSeed();
    srand(seed);
    SetCompanies();
    SimulateToSavePoint();
    //GenerateEvents();
    GenerateDataForCompanies();


    return NULL;

}


void GenerateDataForCompanies() 
{

    for (size_t i = 0;i < sim_data.companies->num_elements;i++)
        SimulationLoop(i);

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

void SetNewPrice(SimulationFrame *frame, unsigned int idx)
{

    if (frame->event_price_diff > 0)
        frame->price = frame->last_price + frame->event_price_diff;
    else
        frame->price = frame->last_price + GenerateRandomPriceFluctuation(frame->last_price);
    
    frame->last_price = frame->price; 

}

void SimulationLoop(unsigned int idx) 
{

    /*
    SimulationFrame frame;

    frame.last_price       = sim_data.companies[idx].ipo;
    frame.price            = 0.0;
    frame.current_time     = 0;
    frame.event            = NULL;
    frame.event_end_time   = 0;
    frame.event_price_diff = 0;

    srand(sim_data.companies[idx].company_id + seed);
    StoreStockPrice(sim_data.prices[idx], frame.last_price, frame.current_time);

    while (ShouldContinueSimulation(frame.current_time)) {

        frame.current_time += HOUR;  
        SetNewPrice(&frame, idx);
        StoreStockPrice(&sim_data.prices[idx], frame.price, frame.current_time);

    }
    */

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

int GetCompanySimIndex(char *company_name)
{

    /*

    for (size_t i = 0; i < sim_data.num_companies;i++)
        if (strcmp(company_name, sim_data.companies[i].company_name) == 0)
            return i;

    */

    return -1;

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

int GetNumToReducePricesBy(StockPrices *prices) 
{

    int reduce_by        = 1;
    const int num_prices = prices->num_prices;
    for (size_t i = 0; i < 24;i++) {


        if (TRUNCATE_TO_AMOUNT/((float)num_prices/reduce_by) > 1.0)
            return reduce_by;

        reduce_by = reduce_by << 1;

    }
    return 4;

}

void RemoveElements(StockPrices *prices)
{

    /*

    int reduce_by = GetNumToReducePricesBy(prices);
    int new_index = 1;
    for (size_t i = 1; i < prices->num_prices;i++) {

        if ((i + 1) % reduce_by == 0 || (i + 1) == prices->num_prices) {

            prices->prices[new_index] = prices->prices[i];
            prices->times[new_index]  = prices->times[i];
            new_index++;

        }

    }
    prices->num_prices = new_index;

    */

}

void ReduceStockPriceAmount(StockPrices *prices)
{

    /*

    if (prices == NULL)
        return;

    if (TRUNCATE_TO_AMOUNT/(float)prices->num_prices > 1.0)
        return;

    RemoveElements(prices);

    */

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

void Simulation_LoadModifiers() 
{

    modifiers = DBCards_GetPlayedModifiersCopy();

}

void Simulation_SimulateUntil(time_t t)
{

    step_time = t;
    atomic_store(&simulation_finished, true);

}

void Simulation_Init(uint32_t new_game_seed, uint32_t new_save_id)
{

    save_id = new_save_id;
    seed = new_game_seed;
    srand(seed);
    Simulation_LoadModifiers();

}

void Simulation_Reset()
{

    atomic_store(&simulation_finished, false);
    save_id   = 0;
    seed      = 0;
    step_time = 0;

}

void Simulation_SimulateStep()
{



}

void StopSimulation()
{

    atomic_store(&simulation_finished, false);
    //CleanSimulation();

}