
#include <time.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "log.h"
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

    char *timespan;
    StockPrices *stocks;

} GraphCacheElement;

typedef struct GraphCache {

    char **company_names;
    GraphCacheElement **elements;

} GraphCache;


typedef struct TimeSpan 
{

    char *timespan;
    const time_t diff;

} TimeSpan;

#define NUM_TIMESPANS 11

// 1 day, 3 days, 1 week, 2 weeks, 1 month, 3 months, 6 months, 1 year, 2 years, 5 years, all time
//static const char *timespans[NUM_TIMESPANS] = {"1 day", "3 days", "1 week", "2 weeks", "1 month", "3 months", "6 months", "1 year", "2 years", "5 years", "all time"};

static const TimeSpan timespans[] = {
    {"1 day", 86400},
    {"3 days", 259200},
    {"1 week", 604800},
    {"2 weeks", 1209600},
    {"1 month", 2419200},
    {"3 months", 7257600},
    {"6 months", 14515200},
    {"1 year", 29030400},
    {"2 years", 58060800},
    {"5 years", 145152000},
    {"all time", -1}
};

static GraphCache graph_cache;


static ALLEGRO_THREAD *graph_cache_thread      = NULL;
static ALLEGRO_MUTEX  *graph_cache_mutex       = NULL;
static Company *companies                      = NULL;
static unsigned int num_companies              = 0;

void *GraphEntry(ALLEGRO_THREAD *thread, void *arg);

void InitializeGraphCaching() 
{

    graph_cache_mutex  = al_create_mutex();
    graph_cache_thread = al_create_thread(&GraphEntry, NULL);
    al_start_thread(graph_cache_thread);

}

void UpdateGraphCache() 
{

    // TODO

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
    LogF("time string = %s", buffer);
    return buffer;

}

char *GetTimeSpanDiff(char buffer[128], time_t a_time, const char *timespan) 
{

    for (unsigned int i = 0; i < NUM_TIMESPANS; i++) {

        if (strcmp(timespan, timespans[i].timespan) == 0) {

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
    char temp_time_buff[128];

    GetTimeString(current_time_buff, current_time);
    LogF("Current game time = %s", current_time_buff);

    for (unsigned int i = 0; i < num_companies; i++) {

        for (unsigned int j = 0; j < NUM_TIMESPANS;j++) {

            graph_cache.elements[i][j].stocks = GetStockPricesBetweenRange(graph_cache.company_names[i], GetTimeSpanDiff(temp_time_buff, current_time, graph_cache.elements[i][j].timespan), current_time_buff);

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

void InitializeGraphCache() 
{

    WaitForSimulation();
    if (ShouldICleanUp())
        return;

    companies                 = GetAllCompanies();
    num_companies             = GetNumCompanies();
    graph_cache.company_names = malloc(sizeof(char *) * num_companies);
    graph_cache.elements      = malloc(sizeof(GraphCacheElement *) * num_companies);
    for (unsigned int i = 0; i < num_companies;i++) {

        graph_cache.elements[i] = malloc(sizeof(GraphCacheElement) * NUM_TIMESPANS);
        for (unsigned int j = 0; j < NUM_TIMESPANS; j++) {

            graph_cache.elements[i][j].timespan = timespans[j].timespan;
            graph_cache.elements[i][j].stocks    = NULL;

        }
        graph_cache.company_names[i] = companies[i].company_name;
        LogF("graph cache name set = %s", graph_cache.company_names[i]);

    }

    GenerateNewGraphCache();

}

void *GraphEntry(ALLEGRO_THREAD *thread, void *arg) 
{

    InitializeGraphCache();

    time_t cache_time = GetGameTime();
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