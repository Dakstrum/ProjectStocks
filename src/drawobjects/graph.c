
#include <time.h>
#include <stdbool.h>
#include <stdatomic.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "log.h"
#include "graph.h"
#include "shared.h"
#include "account.h"
#include "drawobject.h"
#include "simulation.h"

typedef struct TimeSpanWithDiff 
{

    TimeSpan timespan;
    const time_t diff;

} TimeSpanWithDiff;

#define NUM_TIMESPANS 11
static const TimeSpanWithDiff timespans[] = 
{
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

    object->graph.next_refresh = GetOffsetTime(75);
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

DrawObject *PollForNewGraphObject(DrawObject *object) 
{

    if (IsTimeSpecInPast(&object->graph.next_refresh)) {

        free(object->graph.points);
        DrawObject *graph_object   = GetGraphDrawObject(object->graph.company, object->graph.timespan, object->width, object->height);
        object->graph              = graph_object->graph;
        object->graph.next_refresh = GetOffsetTime(75);
        free(graph_object);

    }
    return object;

}

void DrawGraph(DrawObject *object) 
{

    object = PollForNewGraphObject(object);
    Point *points = object->graph.points;

    if (points == NULL || object->graph.num_points == 0)
        return;

    const float x             = object->x;
    const float y_start_point = object->y + object->height;
    ALLEGRO_COLOR color       = al_map_rgba(255, 255, 255, 255);
    for (unsigned short int i = 0;i < object->graph.num_points - 1;i++)
        al_draw_line(x + points[i].x, y_start_point - points[i].y, x + points[i+1].x, y_start_point - points[i+1].y, color , 2);

}
