
#include <time.h>
#include <stdbool.h>
#include <stdatomic.h>

#include <allegro5/allegro.h>

#include "log.h"
#include "graph.h"
#include "shared.h"
#include "account.h"
#include "drawlayers.h"
#include "dbaccess.h"
#include "simulation.h"
#include "jsoncompanies.h"

typedef struct GraphQueue {

    StockPrices *stocks;

    char *name;
    char *timespan;

    int height;
    int width;

    bool can_delete;
    bool retrieved;

    time_t delete_timeout;

} GraphQueue;

typedef struct GraphCacheElement {

    TimeSpan timespan;
    StockPrices *stocks;

} GraphCacheElement;

typedef struct GraphCache {

    char **company_names;
    GraphCacheElement **elements;

} GraphCache;

typedef struct TimeSpanWithDiff 
{

    TimeSpan timespan;
    const time_t diff;

} TimeSpanWithDiff;

#define NUM_TIMESPANS 11
static const TimeSpanWithDiff timespans[] = {
    {ONE_DAY     , 86400},
    {THREE_DAYS  , 259200},
    {ONE_WEEK    , 604800},
    {TWO_WEEKS   , 1209600},
    {ONE_MONTH   , 2419200},
    {THREE_MONTHS, 7257600},
    {SIX_MONTHS  , 14515200},
    {ONE_YEAR    , 29030400},
    {TWO_YEARS   , 58060800},
    {FIVE_YEARS  , 145152000},
    {ALL_TIME    , -1}
};

static GraphCache exclusive_graph_cache;
static GraphCache threaded_graph_cache;

static ALLEGRO_THREAD *graph_cache_thread      = NULL;
static ALLEGRO_MUTEX  *graph_cache_mutex       = NULL;
static Company *companies                      = NULL;
static unsigned int num_companies              = 0;

static atomic_bool graph_cache_ready;

void *GraphEntry(ALLEGRO_THREAD *thread, void *arg);

bool IsGraphCacheReady() 
{

    return atomic_load(&graph_cache_ready);

}

void InitializeGraphCaching() 
{

    atomic_store(&graph_cache_ready, false);
    graph_cache_mutex  = al_create_mutex();
    graph_cache_thread = al_create_thread(&GraphEntry, NULL);
    al_start_thread(graph_cache_thread);

}

int GetCompanyIndex(char *company_name) 
{

    for (unsigned int i = 0; i < num_companies; i++)
        if (strcmp(company_name, threaded_graph_cache.company_names[i]) == 0)
            return i;

    return -1;

}

DrawObject *GetBasicGraphDrawObject(int width, int height, int num_points) 
{

    DrawObject *object           = CreateNewDrawObject();
    object->type                 = GRAPH;
    object->width                = width;
    object->height               = height;
    object->should_this_be_drawn = true;
    object->name                 = NULL;
    object->asset_path           = NULL;
    object->child_of             = NULL;

    object->graph.next_refresh = time(NULL) + 3;
    object->graph.num_points   = (unsigned int)num_points;
    object->graph.points       = malloc(sizeof(Point) * num_points);

    return object;

}

void SetGraphPoints(DrawObject *object, StockPrices *stocks) 
{

    float point_width_diff   = (float)object->width / stocks->num_prices;
    float min_price          = MinF(stocks->prices, stocks->num_prices);
    float max_min_price_diff = MaxMinDiff(stocks->prices, stocks->num_prices);

    for (unsigned int i = 0; i < stocks->num_prices;i++) {

        object->graph.points[i].x = point_width_diff*i;
        object->graph.points[i].y = ((stocks->prices[i] - min_price)/(max_min_price_diff))*object->height;

    }

}

DrawObject *GetConstructedGraphDrawObject(int company_index, int timespan_index, int width, int height) 
{

    StockPrices *stocks  = threaded_graph_cache.elements[company_index][timespan_index].stocks;
    if (stocks == NULL)
        return NULL;
    DrawObject *object   = GetBasicGraphDrawObject(width, height, stocks->num_prices);
    SetGraphPoints(object, stocks);

    return object;

}

DrawObject *GetGraphDrawObject(char *company_name, TimeSpan timespan, int width, int height) 
{

    DrawObject *graph_object = NULL;

    al_lock_mutex(graph_cache_mutex);

    int company_index  = GetCompanyIndex(company_name);

    if (company_index != -1)
        graph_object = GetConstructedGraphDrawObject(company_index, (int)timespan, width, height);

    al_unlock_mutex(graph_cache_mutex);

    if (graph_object != NULL) {

        graph_object->graph.company  = company_name;
        graph_object->graph.timespan = timespan;

    }

    return graph_object;

}

void UpdateGraphCacheElement(unsigned int i, unsigned int j) 
{

    if (threaded_graph_cache.elements[i][j].stocks != NULL)
        free(threaded_graph_cache.elements[i][j].stocks->prices);
    else 
        threaded_graph_cache.elements[i][j].stocks = malloc(sizeof(StockPrices));
    
    threaded_graph_cache.elements[i][j].stocks->num_prices = exclusive_graph_cache.elements[i][j].stocks->num_prices;
    threaded_graph_cache.elements[i][j].stocks->size       = exclusive_graph_cache.elements[i][j].stocks->size;
    threaded_graph_cache.elements[i][j].stocks->prices     = malloc(sizeof(float) * threaded_graph_cache.elements[i][j].stocks->size);
    
    for (unsigned int k = 0; k < exclusive_graph_cache.elements[i][j].stocks->num_prices; k++)
        threaded_graph_cache.elements[i][j].stocks->prices[k] = exclusive_graph_cache.elements[i][j].stocks->prices[k];

}

void UpdateGraphCache() 
{

    for (unsigned int i = 0; i < num_companies; i++)
        for (unsigned int j = 0; j < NUM_TIMESPANS;j++)
            UpdateGraphCacheElement(i, j);

}

time_t GetTimeDiff(time_t a_time, time_t diff) 
{

    time_t temp = a_time - diff;
    if (temp < 0)
        temp = 0;
    
    return temp;

}

char *GetTimeString(char buffer[128], time_t a_time) 
{

    strftime(buffer, 128, "%Y-%m-%d %H:%M:%S", localtime(&a_time));
    return buffer;

}

char *GetTimeSpanDiff(char buffer[128], time_t a_time, TimeSpan timespan) 
{

    for (unsigned int i = 0; i < NUM_TIMESPANS; i++) {

        if (timespan == timespans[i].timespan) {

            if (timespans[i].diff != -1)
                return GetTimeString(buffer, GetTimeDiff(a_time, timespans[i].diff));
            else
                return GetTimeString(buffer, 0);

        }

    }
    return NULL;

}

void GenerateNewGraphCache() 
{

    time_t current_time = GetGameTime();
    char current_time_buff[128];
    GetTimeString(current_time_buff, current_time);
    
    // Might be useful for more companies
    #pragma omp parallel for
    for (unsigned int i = 0; i < num_companies; i++) {

        char temp_time_buff[128];
        char *timespan = NULL;
        for (unsigned int j = 0; j < NUM_TIMESPANS;j++) {

            if (exclusive_graph_cache.elements[i][j].stocks != NULL) {

                free(exclusive_graph_cache.elements[i][j].stocks->prices);
                free(exclusive_graph_cache.elements[i][j].stocks);

            }
            timespan = GetTimeSpanDiff(temp_time_buff, current_time, exclusive_graph_cache.elements[i][j].timespan);
            exclusive_graph_cache.elements[i][j].stocks = GetStockPricesBetweenRange(exclusive_graph_cache.company_names[i], timespan, current_time_buff, exclusive_graph_cache.elements[i][j].timespan);

        }

    }

}

void WaitForSimulation() 
{

    while (!ShouldICleanUp()) {

        if (GetSimulationDone())
            break;

        al_rest(1.0);

    }

}

void SetInitialCache(GraphCache *cache) 
{

    cache->company_names = malloc(sizeof(char *) * num_companies);
    cache->elements      = malloc(sizeof(GraphCacheElement *) * num_companies);
    for (unsigned int i = 0; i < num_companies;i++) {

        cache->elements[i] = malloc(sizeof(GraphCacheElement) * NUM_TIMESPANS);
        for (unsigned int j = 0; j < NUM_TIMESPANS; j++) {

            cache->elements[i][j].timespan = timespans[j].timespan;
            cache->elements[i][j].stocks    = NULL;

        }
        cache->company_names[i] = companies[i].company_name;

    }

}

void InitializeGraphCache() 
{

    WaitForSimulation();
    if (ShouldICleanUp())
        return;

    companies     = GetAllCompanies();
    num_companies = GetNumCompanies();

    SetInitialCache(&exclusive_graph_cache);

    al_lock_mutex(graph_cache_mutex);
    SetInitialCache(&threaded_graph_cache);
    al_unlock_mutex(graph_cache_mutex);

    GenerateNewGraphCache();

}

void *GraphEntry(ALLEGRO_THREAD *thread, void *arg) 
{

    InitializeGraphCache();
    UpdateGraphCache();
    atomic_store(&graph_cache_ready, true);

    time_t cache_time = 0;
    time_t current_game_time;
    while (!ShouldICleanUp()) {

        al_rest(1);
        current_game_time = GetGameTime();
        
        if (current_game_time != cache_time) {

            cache_time = current_game_time;
            GenerateNewGraphCache();
            al_lock_mutex(graph_cache_mutex);
            UpdateGraphCache();
            al_unlock_mutex(graph_cache_mutex);

        }

    }

    return NULL;

}

void CleanUpGraph() 
{

    al_join_thread(graph_cache_thread, NULL);
    al_destroy_thread(graph_cache_thread);

}