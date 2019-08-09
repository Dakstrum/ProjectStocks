#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro.h>

#include "shared.h"
#include "dbaccess.h"

static time_t current_time  = 0;
static int years_to_lapse   = 0;
static int end_year         = 0;

void SetupTables();
void SimulationLoop();
void InitializeDatabase();
void CleanupBeforeExit();
void IncrementCurrentTimeByHour();
bool ShouldContinueSimulation();
int GetYearFromBuff(char *buff);

void *StockSimulationEntry(ALLEGRO_THREAD *thread, void *arg) 
{

    //SimulationLoop();
    CleanupBeforeExit();

}


void SimulationLoop() 
{

    while (ShouldContinueSimulation()) {

        IncrementCurrentTimeByHour();

    }

}

void IncrementCurrentTimeByHour() 
{

    current_time += 3600;

}

void SetYearLapse(int years_to_lapse) 
{

    years_to_lapse = years_to_lapse + 1; // OFFSET by 1 year to fake the approximate correct amount of time.

    
    char buff[100];
    time_t start_time = 0;

    strftime(buff, sizeof(buff), "%c", localtime(&start_time));
    end_year = GetYearFromBuff(buff) + years_to_lapse;


}

void CleanupBeforeExit() 
{


}

bool ShouldContinueSimulation() 
{

    char current_time_buff[100];
    strftime(current_time_buff, sizeof(current_time_buff), "%c", localtime(&current_time));

    int current_year = GetYearFromBuff(current_time_buff);

    if (current_year >= end_year) {
        return true;
    }

    return false;

}

int GetYearFromBuff(char *buff) 
{

    char year_buff[5];
    sscanf(buff, "%*s %*s %*s %*s %s", year_buff);
    year_buff[4] = '\0';
    return atoi(year_buff);

}