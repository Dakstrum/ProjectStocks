#ifndef STOCKSIMULATION_H
#define STOCKSIMULATION_H

#include "shared.h"
#include "vector.h"

typedef struct StockPrice
{

	float price;
	time_t date;

} StockPrice;

void InitializeSimulation();
bool IsSimulationDone();
void *StockSimulationEntry(ALLEGRO_THREAD *thread, void *arg);
void SetEndYear(time_t end_year);

Vector *GetStockPricesFromNowUntil(char *company_name, time_t span);
void ReduceStockPriceAmount(StockPrices *prices);
float CurrentStockPrice(char *company_name);
float GetCurrentStockChange(char *company_name);

char *GetAnyEventAtTime(time_t event_time);

void StartSimulation();
void StopSimulation();

void Simulation_Init(uint32_t game_seed, uint32_t save_id);
void Simulation_SimulateUntil(time_t t);
void Simulation_SimulateStep();

#endif