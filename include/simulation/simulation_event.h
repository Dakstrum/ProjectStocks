#ifndef SIMULATION_EVENT_H
#define SIMULATION_EVENT_H

#include <stdlib.h>
#include <stdint.h>

#include "vector.h"

typedef struct SimulationEvent {

    time_t timestamp;
    char event[128];

} SimulationEvent;

void Simulation_Event_Init();
void Simulation_Event_Push(char *event, time_t t);
Vector *Simulation_Event_GetEvents(time_t t);
Vector *Simulation_Event_GetLastEvents(time_t t, uint32_t num_events);

bool Simulation_Event_EventChanceCheck(uint16_t seed[3]);
Event *Simulation_Event_GetRandomEvent(uint16_t seed[3]);

#endif