
#include <time.h>
#include <stdbool.h>
#include <stdatomic.h>

#include <allegro5/allegro.h>

#include "log.h"
#include "graph.h"
#include "shared.h"
#include "account.h"
#include "drawlayers.h"
#include "simulation.h"

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
    {ALL_TIME    , 0}
};

DrawObject *GetBasicGraphDrawObject(int width, int height, int num_points) 
{

    DrawObject *object           = CreateNewDrawObject();
    object->type                 = GRAPH;
    object->width                = width;
    object->height               = height;
    object->bit_flags            = SHOULD_BE_DRAWN;
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

DrawObject *GetConstructedGraphDrawObject(char *company_name, int timespan_index, int width, int height) 
{

    StockPrices *stocks = GetStockPricesFromNowUntil(company_name, timespans[timespan_index].diff);
    if (stocks == NULL)
        return NULL;

    ReduceStockPriceAmount(stocks);

    DrawObject *object = GetBasicGraphDrawObject(width, height, stocks->num_prices);
    SetGraphPoints(object, stocks);

    return object;

}

DrawObject *GetGraphDrawObject(char *company_name, TimeSpan timespan, int width, int height) 
{

    DrawObject *graph_object = GetConstructedGraphDrawObject(company_name, (int)timespan, width, height);

    if (graph_object != NULL) {

        graph_object->graph.company  = company_name;
        graph_object->graph.timespan = timespan;

    }

    return graph_object;

}