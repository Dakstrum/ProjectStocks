#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>

#include <sqlite3.h>
#include <allegro5/allegro.h>


#include "log.h"
#include "vector.h"
#include "shared.h"
#include "account.h"
#include "dbaccess.h"
#include "dbevents.h"
#include "dbcompany.h"

#define STOCK_PRICE_SIZE 4096

typedef struct Sim 
{

    Company *companies;
    StockPrices *prices;
    float *random_event_chance;

    unsigned int num_companies;

} Sim;

typedef struct EventSim 
{

    Vector *event_times;
    Vector *events;

} EventSim;

typedef struct EventSimId 
{

    Vector *ids;
    Vector *event_sims;

} EventSimId;

typedef struct SimulationFrame
{

    float last_price;
    float price;
    time_t current_time;
    Event *event;

    time_t event_end_time;
    float event_price_diff;

} SimulationFrame;

static EventSim global_events;
static EventSimId category_events;
static EventSimId company_events; 

static const int HOURS_IN_YEAR      = 8760;
static const int HOURS_IN_HALF_YEAR = HOURS_IN_YEAR / 2;
static const int HOURS_IN_TWO_YEARS = HOURS_IN_YEAR * 2;

static const float TRUNCATE_TO_AMOUNT = 500.0;
static const int HOUR = 3600;
static int end_year   = 0;

static atomic_bool simulation_finished;

static Sim sim_data;
static int save_id       = 0;
static unsigned int seed = 0;

void CleanupBeforeExit();

void SimulationLoop(unsigned int idx);
void GenerateDataForCompanies();
void IncrementCurrentTimeByHour();
bool ShouldContinueSimulation(time_t current_time);

void SetYearLapse(int years_to_lapse);
int GetYearFromBuff(char *buff);

bool IsSimulationDone() 
{

    return atomic_load(&simulation_finished);

}

void InitializeSimulation() 
{

    atomic_store(&simulation_finished, false);

}

void SetRandomSeed() 
{

    if (save_id != -1)
        seed = GetSaveSeed();
    else
        seed = time(NULL);

}

void InitializeStockPrice(StockPrices *prices)
{

    prices->prices     = malloc(sizeof(float)  * STOCK_PRICE_SIZE);
    prices->times      = malloc(sizeof(time_t) * STOCK_PRICE_SIZE);
    prices->size       = STOCK_PRICE_SIZE;
    prices->num_prices = 0;

}

void ReclaimUnusedStockPriceMemory(StockPrices *prices) 
{

    prices->size   = prices->num_prices;
    prices->prices = realloc(prices->prices, sizeof(float) * (prices->size + 1));
    prices->times  = realloc(prices->times, sizeof(time_t) * (prices->size + 1));

}

void SetCompanies() 
{

    sim_data.companies           = GetAllCompanies();
    sim_data.num_companies       = GetNumCompanies();
    sim_data.prices              = malloc(sizeof(StockPrices) * sim_data.num_companies);
    sim_data.random_event_chance = malloc(sizeof(float) * sim_data.num_companies);

    for (size_t i = 0; i < sim_data.num_companies;i++) {

        InitializeStockPrice(&sim_data.prices[i]);
        sim_data.random_event_chance[i] = 0.0;

    }

}

void GenerateGameEvents(EventSim *events, Event *(*GetRandomEvent)())
{

    time_t current_time = 0;
    while (ShouldContinueSimulation(current_time)) {

        int hours_passed = (HOURS_IN_HALF_YEAR + rand() % HOURS_IN_TWO_YEARS) * HOUR;
        current_time    += hours_passed;
        Vector_PushBack(events->event_times, &current_time);
        Vector_PushBack(events->events, GetRandomEvent());

    }

}

void GenerateGameEventsId(EventSim *events, int id, Event *(*GetRandomEvent)(int))
{

    time_t current_time = 0;
    while (ShouldContinueSimulation(current_time)) {

        int hours_passed = (HOURS_IN_HALF_YEAR + rand() % HOURS_IN_TWO_YEARS) * HOUR;
        current_time    += hours_passed;
        Vector_PushBack(events->event_times, &current_time);
        Vector_PushBack(events->events, GetRandomEvent(id));
        LogF("Current_time = %lu, id: %d", current_time, id);

    }

}

void InitializeCategoryEventGeneration()
{

    category_events.ids        = Vector_Create(sizeof(int), 16);
    category_events.event_sims = Vector_Create(sizeof(EventSim), 16);

    int num_categories = GetNumCompanyCategories();
    int *category_ids  = GetCompanyCategoryIds();
    for (int i = 0; i < num_categories;i++) {

        Vector_PushBack(category_events.ids, &category_ids[i]);
        EventSim temp;
        temp.event_times = Vector_Create(sizeof(time_t), 16);
        temp.events      = Vector_Create(sizeof(Event), 16);
        Vector_PushBack(category_events.event_sims, &temp);
        GenerateGameEventsId(&temp, category_ids[i], &GetRandomCategoryEvent);

    }

}

void InitializeCompanyEventGeneration()
{

    company_events.ids        = Vector_Create(sizeof(int), 16);
    company_events.event_sims = Vector_Create(sizeof(EventSim), 16);

    for (unsigned int i = 0; i < sim_data.num_companies;i++) {

        Vector_PushBack(company_events.ids, &sim_data.companies[i].company_id);
        EventSim temp;
        temp.event_times = Vector_Create(sizeof(time_t), 16);
        temp.events      = Vector_Create(sizeof(Event), 16);
        Vector_PushBack(company_events.event_sims, &temp);
        GenerateGameEventsId(&temp, sim_data.companies[i].company_id, &GetRandomCompanyEvent);

    }

}

void InitializeEventGeneration()
{

    global_events.event_times  = Vector_Create(sizeof(time_t), 16);
    global_events.events       = Vector_Create(sizeof(Event), 16);

    GenerateGameEvents(&global_events, GetRandomGlobalEvent);

    InitializeCategoryEventGeneration();
    InitializeCompanyEventGeneration();

}

void GenerateEvents()
{

    srand(seed);
    InitializeEventGeneration();


}

void *StockSimulationEntry(ALLEGRO_THREAD *thread, void *arg) 
{

    SetYearLapse(25);
    save_id = GetSaveId();
    SetRandomSeed();
    SetCompanies();
    GenerateEvents();
    GenerateDataForCompanies();
    atomic_store(&simulation_finished, true);

    return NULL;

}

void GenerateDataForCompanies() 
{

    for (size_t i = 0;i < sim_data.num_companies;i++)
        SimulationLoop(i);

}

void StoreStockPrice(StockPrices *prices, float price, time_t timestamp)
{

    if (prices->num_prices == prices->size) {

        prices->size  += STOCK_PRICE_SIZE;
        prices->prices = realloc(prices->prices, sizeof(float)  * prices->size);
        prices->times  = realloc(prices->times,  sizeof(time_t) * prices->size);

    }
    prices->prices[prices->num_prices] = price;
    prices->times[prices->num_prices]  = timestamp;
    prices->num_prices++;

}

float GetRandomSign()
{

    if (rand()%2 == 0)
        return -1.0;
    else
        return 1.0;

}

float GetRandomFloat()
{

    return (float)rand()/RAND_MAX;

}

float GetRandomEventMagnitude()
{

    float random = GetRandomFloat();

    if (random >= .2)
        return .25;
    else if (random >= 0.05)
        return .5;
    else
        return .75;

}

float GenerateRandomEvent(float last_price, unsigned int idx)
{

    sim_data.random_event_chance[idx] += GetRandomSign() * GetRandomFloat() + .1;
    if (sim_data.random_event_chance[idx] < 0)
        sim_data.random_event_chance[idx] = 0.0;

    if (sim_data.random_event_chance[idx] > 50 + rand() % 50)
        return .05 * last_price * GetRandomSign() * GetRandomFloat() * GetRandomEventMagnitude();

    return 0.0;

}

float GenerateRandomPriceFluctuation(float last_price) 
{

    return GetRandomSign() * GetRandomFloat() * last_price * .01;

}

Event *GetGenericEventOnDay(EventSim *event_sim, time_t current_time)
{

    time_t *times = (time_t *)event_sim->event_times->elements;

    for (size_t i = 0; i < event_sim->event_times->num_elements;i++)
        if (times[i] == current_time)
            return &((Event *)event_sim->events->elements)[i];

    return NULL;

}

Event *GetGenericEventOnDayWithId(EventSimId *event_sim, time_t current_time, unsigned int id)
{

    unsigned int *ids    = event_sim->ids->elements;
    EventSim *event_sims = event_sim->event_sims->elements;

    for (unsigned int i = 0; i < event_sim->ids->num_elements;i++)
        if (ids[i] == id)
            return GetGenericEventOnDay(&event_sims[i], current_time);

    return NULL;

}

Event *GetEventOnDay(time_t current_time, unsigned int idx)
{

    Event *event = NULL;
    event = GetGenericEventOnDay(&global_events, current_time);

    if (event != NULL)
        return event;

    unsigned int company_id = sim_data.companies[idx].company_id;
    event = GetGenericEventOnDayWithId(&company_events, current_time, company_id);

    if (event != NULL)
        return event;

    return NULL;

}

void CheckForEvent(SimulationFrame *frame, unsigned int idx) 
{

    if (frame->event != NULL)
        return;

    frame->event = GetEventOnDay(frame->current_time, idx);
    if (frame->event != NULL) {

        frame->event_end_time   = frame->current_time + frame->event->modifier_length * HOUR  * 24;
        frame->event_price_diff = (frame->last_price * frame->event->price_modifier) / (frame->event->modifier_length * 24);

    }

}

void CheckToEndEvent(SimulationFrame *frame)
{

    if (frame->current_time < frame->event_end_time)
        return;

    frame->event            = NULL;
    frame->event_end_time   = 0;
    frame->event_price_diff = 0;

}

void SetNewPrice(SimulationFrame *frame, unsigned int idx)
{

    if (frame->event_price_diff > 0)
        frame->price = frame->last_price + frame->event_price_diff;
    else
        frame->price = frame->last_price + GenerateRandomPriceFluctuation(frame->last_price) + GenerateRandomEvent(frame->last_price, idx);
    
    frame->last_price = frame->price; 

}

void SimulationLoop(unsigned int idx) 
{

    SimulationFrame frame;

    frame.last_price       = sim_data.companies[idx].ipo;
    frame.price            = 0.0;
    frame.current_time     = 0;
    frame.event            = NULL;
    frame.event_end_time   = 0;
    frame.event_price_diff = 0;

    srand(sim_data.companies[idx].company_id + seed);
    StoreStockPrice(&sim_data.prices[idx], frame.last_price, frame.current_time);

    while (ShouldContinueSimulation(frame.current_time)) {

        frame.current_time += HOUR;  
        CheckForEvent(&frame, idx);
        CheckToEndEvent(&frame);
        SetNewPrice(&frame, idx);
        StoreStockPrice(&sim_data.prices[idx], frame.price, frame.current_time);

    }

    ReclaimUnusedStockPriceMemory(&sim_data.prices[idx]);

}

void SetYearLapse(int years_to_lapse) 
{

    years_to_lapse = years_to_lapse + 1; // OFFSET by 1 year to fake the approximate correct amount of time.
    
    char buff[128];
    time_t start_time = 0;

    strftime(buff, sizeof(buff), "%Y", localtime(&start_time));
    end_year = GetYearFromBuff(buff) + years_to_lapse;


}

bool ShouldContinueSimulation(time_t current_time) 
{

    char current_time_buff[128];
    strftime(current_time_buff, sizeof(current_time_buff), "%Y", localtime(&current_time));

    return GetYearFromBuff(current_time_buff) < end_year;

}

int GetYearFromBuff(char *buff) 
{

    char year_buff[5];
    sscanf(buff, "%s", year_buff);
    year_buff[4] = '\0';
    return atoi(year_buff);

}

int GetCompanySimIndex(char *company_name)
{

    for (size_t i = 0; i < sim_data.num_companies;i++)
        if (strcmp(company_name, sim_data.companies[i].company_name) == 0)
            return i;

    return -1;

}

StockPrices *GetStockPricesFromNowUntil(char *company_name, time_t span)
{

    int company_idx = GetCompanySimIndex(company_name);
    if (company_idx == -1)
        return NULL;

    time_t current_time  = GetGameTime();
    time_t previous_time = current_time - span;

    if (span == 0)
        previous_time = 0;

    if (previous_time < 0)
        previous_time = 0;

    StockPrices *prices = malloc(sizeof(StockPrices));
    InitializeStockPrice(prices);
    for (size_t i = 0; i < sim_data.prices[company_idx].num_prices;i++) {

        if (sim_data.prices[company_idx].times[i] > current_time)
            break;

        if (sim_data.prices[company_idx].times[i] >= previous_time && sim_data.prices[company_idx].times[i] <= current_time)
            StoreStockPrice(prices,sim_data.prices[company_idx].prices[i], sim_data.prices[company_idx].times[i]);

    }
    ReclaimUnusedStockPriceMemory(prices);

    return prices;

}

int GetNumToReducePricesBy(StockPrices *prices) 
{

    int reduce_by        = 1;
    const int num_prices = prices->num_prices;
    for (size_t i = 0; i < 24;i++) {


        if (TRUNCATE_TO_AMOUNT/((float)num_prices/reduce_by) > 1.0)
            return reduce_by;

        reduce_by = reduce_by << 1;

    }
    return 4;

}

void RemoveElements(StockPrices *prices)
{

    int reduce_by = GetNumToReducePricesBy(prices);
    int new_index = 1;
    for (size_t i = 1; i < prices->num_prices;i++) {

        if ((i + 1) % reduce_by == 0 || (i + 1) == prices->num_prices) {

            prices->prices[new_index] = prices->prices[i];
            prices->times[new_index]  = prices->times[i];
            new_index++;

        }

    }
    prices->num_prices = new_index;
    ReclaimUnusedStockPriceMemory(prices);

}

void ReduceStockPriceAmount(StockPrices *prices)
{

    if (prices == NULL)
        return;

    if (TRUNCATE_TO_AMOUNT/(float)prices->num_prices > 1.0)
        return;

    RemoveElements(prices);

}

float CurrentStockPrice(char *company_name) 
{

    int company_idx = GetCompanySimIndex(company_name);
    if (company_idx == -1)
        return -1.0f;

    time_t current_time = GetGameTime(); 
    for (size_t i = 0; i < sim_data.prices[company_idx].num_prices; i++)
        if (sim_data.prices[company_idx].times[i] == current_time)
            return sim_data.prices[company_idx].prices[i];

    return -1.0f;

}

float GetCurrentStockChange(char *company_name)
{
    static float last_price    = 0.0;
    static float current_price = 0.0;
    static float change        = 0.0;

    if(current_price != CurrentStockPrice(company_name))
        last_price = current_price;

    current_price = CurrentStockPrice(company_name);
    change        = current_price - last_price;
    
    return change;
}

static ALLEGRO_THREAD *stock_simulation_thread = NULL;

void StartSimulation()
{

    stock_simulation_thread = al_create_thread(StockSimulationEntry, NULL);
    al_start_thread(stock_simulation_thread);

}

void CleanUpEvents(EventSimId *event, int num_elements)
{

    EventSim *temp = (EventSim *)event->event_sims->elements;
    for (int i = 0; i < num_elements;i++) {

        Vector_Delete(temp[i].event_times);
        Vector_Delete(temp[i].events);

    }
    Vector_Delete(event->event_sims);
    Vector_Delete(event->ids);

}

void CleanSimulation()
{

    Vector_Delete(global_events.event_times);
    Vector_Delete(global_events.events);

    CleanUpEvents(&category_events, GetNumCompanyCategories());
    CleanUpEvents(&company_events, sim_data.num_companies);

    free(sim_data.prices);
    free(sim_data.random_event_chance);
    sim_data.num_companies       = 0;
    sim_data.prices              = NULL;
    sim_data.random_event_chance = NULL;

}

void StopSimulation()
{

    if (stock_simulation_thread == NULL)
        return;

    al_join_thread(stock_simulation_thread, NULL);
    al_destroy_thread(stock_simulation_thread);
    stock_simulation_thread = NULL;
    atomic_store(&simulation_finished, false);
    CleanSimulation();

}