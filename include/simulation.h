#ifndef STOCKSIMULATION_H
#define STOCKSIMULATION_H

#include "shared.h"

void InitializeSimulation();
bool GetSimulationDone();
void *StockSimulationEntry(ALLEGRO_THREAD *thread, void *arg);
void SetEndYear(time_t end_year);

StockPrices *GetStockPricesFromNowUntil(char *company_name, time_t span);

#endif