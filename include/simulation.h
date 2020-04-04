#ifndef STOCKSIMULATION_H
#define STOCKSIMULATION_H

#include "shared.h"

void InitializeSimulation();
bool IsSimulationDone();
void *StockSimulationEntry(ALLEGRO_THREAD *thread, void *arg);
void SetEndYear(time_t end_year);

StockPrices *GetStockPricesFromNowUntil(char *company_name, time_t span);
void ReduceStockPriceAmount(StockPrices *prices);
float CurrentStockPrice(char *company_name);
float GetCurrentStockChange(char *company_name);

void StartSimulation();
void StopSimulation();
#endif