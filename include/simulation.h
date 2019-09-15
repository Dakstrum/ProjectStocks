#ifndef STOCKSIMULATION_H
#define STOCKSIMULATION_H

void InitializeSimulation();
void GetSimulationDone();
void *StockSimulationEntry(ALLEGRO_THREAD *thread, void *arg);
void SetEndYear(time_t end_year);

#endif