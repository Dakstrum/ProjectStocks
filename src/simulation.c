#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>

#include <sqlite3.h>
#include <allegro5/allegro.h>


#include "log.h"
#include "shared.h"
#include "account.h"
#include "dbaccess.h"
#include "jsoncompanies.h"

typedef struct Sim {

    Company *companies;
    StockPrices *prices;

    unsigned int num_companies;

} Sim;


static const int HOUR = 3600;
static int end_year   = 0;

static atomic_bool simulation_finished;

static Sim sim_data;
static int save_id       = 0;
static unsigned int seed = 0;

void CleanupBeforeExit();

void SimulationLoop(unsigned int idx);
void GenerateDataForCompanies();
void IncrementCurrentTimeByHour();
bool ShouldContinueSimulation(long long int current_time);

void SetYearLapse(int years_to_lapse);
int GetYearFromBuff(char *buff);

float GenerateRandomPriceFluctuation(float last_price, unsigned int *thread_seed);

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

    if (save_id != -1)
        seed = GetSaveSeed();
    else
        seed = time(NULL);

}

void InitializeStockPrice(StockPrices *prices)
{

    prices->prices     = malloc(sizeof(float)  * 128);
    prices->times      = malloc(sizeof(time_t) * 128);
    prices->size       = 128;
    prices->num_prices = 0;

}

void ReclaimUnusedStockPriceMemory(StockPrices *prices) 
{

    prices->size = prices->num_prices;
    prices->prices = realloc(prices->prices, sizeof(float) * prices->size);
    prices->times  = realloc(prices->times, sizeof(time_t) * prices->size);

}

void SetCompanies() 
{

    sim_data.companies     = GetAllCompanies();
    sim_data.num_companies = GetNumCompanies();
    sim_data.prices        = malloc(sizeof(StockPrices) * sim_data.num_companies);

    for (int i = 0; i < sim_data.num_companies;i++)
        InitializeStockPrice(&sim_data.prices[i]);

}

void *StockSimulationEntry(ALLEGRO_THREAD *thread, void *arg) 
{

    SetYearLapse(25);
    save_id = GetSaveId();
    SetRandomSeed();
    SetCompanies();
    GenerateDataForCompanies();
    atomic_store(&simulation_finished, true);

    return NULL;

}

void GenerateDataForCompanies() 
{

    #pragma omp parallel for
    for (unsigned int i = 0;i < sim_data.num_companies;i++)
        SimulationLoop(i);

}

void StoreStockPrice(StockPrices *prices, float price, time_t timestamp)
{

    if (prices->num_prices == prices->size) {

        prices->size  += 128;
        prices->prices = realloc(prices->prices, sizeof(float)  * prices->size);
        prices->times  = realloc(prices->times,  sizeof(time_t) * prices->size);

    }
    prices->prices[prices->num_prices] = price;
    prices->times[prices->num_prices]  = timestamp;
    prices->num_prices++;

}

void SimulationLoop(unsigned int idx) 
{

    float last_price           = sim_data.companies[idx].ipo;
    float price                = 0.0;
    long long int current_time = 0;
    unsigned int thread_seed   = sim_data.companies[idx].company_id + seed;

    char time_buff[128];
    strftime(time_buff, sizeof(time_buff), "%Y-%m-%d %H:%M:%S", localtime(&current_time));

    StoreStockPrice(&sim_data.prices[idx], last_price, current_time);
    while (ShouldContinueSimulation(current_time)) {

        current_time += HOUR;
        price         = last_price + GenerateRandomPriceFluctuation(last_price, &thread_seed);
        last_price    = price;

        strftime(time_buff, sizeof(time_buff), "%Y-%m-%d %H:%M:%S", localtime(&current_time));
        StoreStockPrice(&sim_data.prices[idx], price, current_time);

    }
    ReclaimUnusedStockPriceMemory(&sim_data.prices[idx]);

}

float GenerateRandomPriceFluctuation(float last_price, unsigned int *thread_seed) 
{

    return (1+rand()%2);

}


void SetYearLapse(int years_to_lapse) 
{

    years_to_lapse = years_to_lapse + 1; // OFFSET by 1 year to fake the approximate correct amount of time.
    
    char buff[128];
    time_t start_time = 0;

    strftime(buff, sizeof(buff), "%Y", localtime(&start_time));
    end_year = GetYearFromBuff(buff) + years_to_lapse;


}

bool ShouldContinueSimulation(long long int current_time) 
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

    for (int i = 0; i < sim_data.num_companies;i++)
        if (strcmp(company_name, sim_data.companies[i].company_name) == 0)
            return i;

    return -1;

}

StockPrices *GetStockPricesFromNowUntil(char *company_name, time_t span)
{

    int company_idx = GetCompanySimIndex(company_name);
    if (company_idx == -1)
        return NULL;

    time_t current_time  = GetGameTime();
    time_t previous_time = current_time - span;
    if (previous_time < 0)
        previous_time = 0;

    StockPrices *prices = malloc(sizeof(StockPrices));
    InitializeStockPrice(prices);
    for (int i = 0; i < sim_data.prices[company_idx].num_prices;i++) {

        if (sim_data.prices[company_idx].times[i] > current_time)
            break;

        if (sim_data.prices[company_idx].times[i] >= previous_time && sim_data.prices[company_idx].times[i] <= current_time)
            StoreStockPrice(prices,sim_data.prices[company_idx].prices[i], sim_data.prices[company_idx].times[i]);

    }
    ReclaimUnusedStockPriceMemory(prices);

    return prices;

}