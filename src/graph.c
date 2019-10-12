
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "log.h"
#include "shared.h"
#include "dbaccess.h"

typedef struct GraphQueue {

    StockPrices *stocks;

    char *name;
    char *timespan;

    int height;
    int width;

    bool can_delete;
    bool retrieved;

} GraphQueue;

static GraphQueue *graph_queue           = NULL;
static const unsigned int max_queue_size = 16;

static ALLEGRO_THREAD *queue_thread      = NULL;
static ALLEGRO_MUTEX  *queue_mutex       = NULL;

void *GraphEntry(ALLEGRO_THREAD *thread, void *arg);

void ResetGraphQueueAtIndex(unsigned int i) 
{

    graph_queue[i].stocks     = NULL;
    graph_queue[i].name       = NULL;
    graph_queue[i].timespan   = NULL;
    graph_queue[i].can_delete = false;
    graph_queue[i].retrieved  = false;

}

void ResetGraphQueue() 
{

    for (unsigned int i = 0; i < max_queue_size; i++)
        ResetGraphQueueAtIndex(i);

}


void InitializeGraphQueue() 
{

    graph_queue  = malloc(sizeof(GraphQueue) * max_queue_size);

    ResetGraphQueue();

    queue_mutex  = al_create_mutex();
    queue_thread = al_create_thread(&GraphEntry, NULL);
    al_start_thread(queue_thread);

}

int RequestForGraph(char *name, char *timespan, int height, int width) 
{

    if (name == NULL || timespan == NULL)
        return -1;



    return -1;

}


void SetGraphData(char *name, char *start_time, char *end_time) 
{

    GetStockPricesBetweenRange("WeBeHard", "1969-12-31 18:00:00", "1970-01-01 18:00:00");

}

void SetGraphInformationFromQueue() 
{

    al_lock_mutex(queue_mutex);
    for (unsigned int i = 0; i < max_queue_size; i++) {

        if (graph_queue[i].name != NULL && graph_queue[i].retrieved == false) {

            graph_queue[i].stocks    = GetStockPricesBetweenRange(graph_queue[i].name, "1969-12-31 18:00:00", "1970-01-01 18:00:00");
            graph_queue[i].retrieved = true;

        }

    }
    al_unlock_mutex(queue_mutex);

}

void ClearNoLongerUsedGraphInformation() 
{

    al_lock_mutex(queue_mutex);

    for (unsigned int i = 0; i < max_queue_size; i++) {

        if (graph_queue[i].stocks != NULL && graph_queue[i].can_delete == true) {

            free(graph_queue[i].stocks->prices);
            free(graph_queue[i].stocks);
            ResetGraphQueueAtIndex(i);

        }

    }

    al_unlock_mutex(queue_mutex);

}

void *GraphEntry(ALLEGRO_THREAD *thread, void *arg) 
{

    while (!ShouldICleanUp()) {

        al_rest(1);
        SetGraphInformationFromQueue();
        ClearNoLongerUsedGraphInformation();

    }

    return NULL;

}