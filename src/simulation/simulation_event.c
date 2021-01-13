
#include <string.h>

#include "log.h"
#include "shared.h"
#include "dbcompany.h"
#include "simulation_event.h"

static Vector *events = NULL;
static const float EVENT_CHANCE = 4.0/(24.0 * 30.0);

static uint32_t uid = 1;

void Simulation_Event_Init()
{

    if (events != NULL) {

        uid = 1;
        Vector_Reset(events);

    } else {

        events = Vector_Create(sizeof(SimulationEvent), 128);

    }

}

void Simulation_Event_Push(char *event, time_t t) 
{

    if (event == NULL)
        return;

    SimulationEvent temp;
    temp.timestamp = t;
    strncpy(temp.event, event, 127);
    temp.event[127] = '\0';
    temp.uid = uid;

    uid++;

    Vector_PushBack(events, &temp);

}

Vector *Simulation_Event_GetEvents(time_t t) 
{

    Vector *time_events = Vector_Create(sizeof(SimulationEvent), 8);

    SimulationEvent *temp_events = events->elements;
    for (size_t i = 0; i < events->num_elements;i++) {


        if (temp_events[i].timestamp == t)
            Vector_PushBack(time_events, &temp_events[i]);

    }

    return time_events;

}

uint32_t Simulation_Event_GetLastEventsStartIndex(time_t t) 
{

    SimulationEvent *temp_events = events->elements;
    for (size_t i = 0; i < events->num_elements;i++) {

        if (temp_events[i].timestamp == t)
            return i;
        else if (temp_events[i].timestamp > t)
            return i - 1;

    }
    return 0;

}

Vector *Simulation_Event_GetLastEventsT(time_t t, uint32_t num_events)
{

    uint32_t start_idx = Simulation_Event_GetLastEventsStartIndex(t);

    Vector *temp = Vector_Create(sizeof(SimulationEvent), 4);
    SimulationEvent *temp_events = events->elements;

    int32_t end_idx = (int32_t)start_idx - (int32_t)num_events;

    if (end_idx < 0)
        end_idx = 0;

    for (size_t i = start_idx; i > (uint32_t)end_idx;i--)
        Vector_PushBack(temp, &temp_events[i]);

    return temp;

}

Vector *Simulation_Event_GetLastEvents(uint32_t num_events)
{

    Vector *temp = Vector_Create(sizeof(SimulationEvent), 4);
    SimulationEvent *temp_events = events->elements;

    uint32_t end_idx = events->num_elements - num_events;

    if (events->num_elements < num_events)
        end_idx = 0;

    for (size_t i = events->num_elements;i > end_idx; i--)
        Vector_PushBack(temp, &temp_events[i-1]);

    return temp;

}

bool Simulation_Event_EventChanceCheck(uint16_t seed[3])
{

    return EVENT_CHANCE >= shared_get_random_float(seed);

}

Event *Simulation_Event_GetRandomEvent(uint16_t seed[3])
{

    float event_type_prob = shared_get_random_float(seed);
    if (event_type_prob <= 0.33)
        return dbevents_get_company_event(1 + shared_nrand48(seed) % (GetNumCompanies()), seed);
    else if (event_type_prob <= 0.66)
        return dbevents_get_category_event(1 + shared_nrand48(seed) % (dbevents_get_num_categories()), seed);
    else 
        return dbevents_get_global_event(seed);

}