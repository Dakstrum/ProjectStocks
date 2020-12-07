#ifndef SIMULATION_EVENT_H
#define SIMULATION_EVENT_H

#include <stdint.h>

#include "vector.h"

void Simulation_Event_Push(char *event, time_t t);
void Simulation_Event_Clear();

Vector *Simulation_Event_GetEvents(time_t t);
Vector *Simulation_Event_GetLastEvents(time_t t, uint32_t num_events);

#endif