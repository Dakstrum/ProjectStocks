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

static const int HOUR       = 3600;
static int end_year         = 0;

static atomic_bool simulation_finished;

static Company *companies;
static int save_id                = 0;
static unsigned int seed          = 0;
static unsigned int num_companies = 0;

void CleanupBeforeExit();

void GenerateSimulation();
void SimulationLoop(sqlite3 *db, unsigned int idx);
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

void *StockSimulationEntry(ALLEGRO_THREAD *thread, void *arg) 
{

    GenerateSimulation();
    return NULL;

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

    companies     = GetAllCompanies();
    num_companies = GetNumCompanies();

}

void GenerateSimulation()
{

    SetYearLapse(25);
    if (ShouldICleanUp())
        return;

    save_id = GetSaveId();
    SetRandomSeed();
    SetCompanies();
    GenerateDataForCompanies();
    atomic_store(&simulation_finished, true);
}

void GenerateDataForCompanies() 
{

    // Could make multithreaded, but need to use rand_r for thread safety.
    // Will see how it performs with a single thread.
    sqlite3 *db;
    if (OpenConnection(&db) != 0)
        return;

    for (unsigned int i = 0;i < num_companies;i++) {

        if (ShouldICleanUp())
            return;

        sqlite3_exec(db, "BEGIN TRANSACTION", NULL, 0, 0);
        SimulationLoop(db, i);
        sqlite3_exec(db, "END TRANSACTION", NULL, 0, 0);

    }
    sqlite3_close(db);

}

void SimulationLoop(sqlite3 *db, unsigned int idx) 
{

    int company_id             = companies[idx].company_id;
    float last_price           = companies[idx].ipo;
    float price                = 0.0;
    long long int current_time = 0;
    unsigned int thread_seed   = companies[idx].company_id + seed;

    char time_buff[128];
    strftime(time_buff, sizeof(time_buff), "%Y-%m-%d %H:%M:%S", localtime(&current_time));

    InsertStockPrice(save_id, company_id, last_price, time_buff, db);
    while (ShouldContinueSimulation(current_time)) {

        current_time += HOUR;
        price         = last_price + GenerateRandomPriceFluctuation(last_price, &thread_seed);
        last_price    = price;

        strftime(time_buff, sizeof(time_buff), "%Y-%m-%d %H:%M:%S", localtime(&current_time));
        InsertStockPrice(save_id, company_id, last_price, time_buff, db);

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