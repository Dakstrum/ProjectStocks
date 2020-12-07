#ifndef SIMULATION_EVENT_H
#define SIMULATION_EVENT_H

#include <stdint.h>

#include "vector.h"

typedef struct SimulationEvent {

    time_t timestamp;
    char event[128];

} SimulationEvent;

void Simulation_Event_Push(char *event, time_t t);
Vector *Simulation_Event_GetEvents(time_t t);
Vector *Simulation_Event_GetLastEvents(time_t t, uint32_t num_events);

#endif