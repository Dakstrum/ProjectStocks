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

bool GetSimulationDone() 
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

void SetCompanies() 
{

    sim_data.companies     = GetAllCompanies();
    sim_data.num_companies = GetNumCompanies();
    sim_data.prices        = malloc(sizeof(StockPrices) * sim_data.num_companies);

    for (int i = 0; i < sim_data.num_companies;i++) {

        sim_data.prices[i].prices     = malloc(sizeof(float) * 128);
        sim_data.prices[i].size       = 128;
        sim_data.prices[i].num_prices = 0;

    }

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

    for (unsigned int i = 0;i < sim_data.num_companies;i++)
        SimulationLoop(i);

}

void StoreStockPrice(unsigned int idx, float price)
{

    if (sim_data.prices[idx].num_prices == sim_data.prices[idx].size) {

        sim_data.prices[idx].size  += 128;
        sim_data.prices[idx].prices = realloc(sim_data.prices[idx].prices, sizeof(float) * sim_data.prices[idx].size);

    }
    sim_data.prices[idx].prices[sim_data.prices[idx].num_prices] = price;
    sim_data.prices[idx].num_prices++;

}

void SimulationLoop(unsigned int idx) 
{

    float last_price           = sim_data.companies[idx].ipo;
    float price                = 0.0;
    long long int current_time = 0;
    unsigned int thread_seed   = sim_data.companies[idx].company_id + seed;

    char time_buff[128];
    strftime(time_buff, sizeof(time_buff), "%Y-%m-%d %H:%M:%S", localtime(&current_time));

    StoreStockPrice(idx, last_price);
    while (ShouldContinueSimulation(current_time)) {

        current_time += HOUR;
        price         = last_price + GenerateRandomPriceFluctuation(last_price, &thread_seed);
        last_price    = price;

        strftime(time_buff, sizeof(time_buff), "%Y-%m-%d %H:%M:%S", localtime(&current_time));
        StoreStockPrice(idx, price);

    }

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

    int current_year = GetYearFromBuff(current_time_buff);

    if (current_year >= end_year) {
        return false;
    }

    return true;

}

int GetYearFromBuff(char *buff) 
{

    char year_buff[5];
    sscanf(buff, "%s", year_buff);
    year_buff[4] = '\0';
    return atoi(year_buff);

}