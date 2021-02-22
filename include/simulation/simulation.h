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

Vector *Simulation_GetStockPrices(char *company_name, time_t current_time, time_t span);
float Simulation_GetLastStockPrice(char *company_name);
float Simulation_GetStockPriceDiff(char *company_name);

void Simulation_Init(uint32_t new_game_seed);
void Simulation_SimulateUntil(time_t t);
void Simulation_SimulateStep(time_t t);
void Simulation_Reset();

void Simulation_ModifyCompany(uint32_t company_id, time_t play_time, float modifier, uint32_t days, char *event);
void Simulation_ModifyCategory(uint32_t category_id, time_t play_time, float modifier, uint32_t days, char *event);
void Simulation_ModifyGlobal(float modifier, time_t play_time, uint32_t days, char *event);

#endif