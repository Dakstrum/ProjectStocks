
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
#include "game.h"

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

float Graph_GetMinPrice(Vector *stocks) 
{

    StockPrice *prices = stocks->elements;
    float min_price    = prices[0].price;
    for (size_t i = 1; i < stocks->num_elements;i++) {

        if (prices[i].price < min_price)
            min_price = prices[i].price;

    }
    return min_price;

}

float Graph_GetMaxPrice(Vector *stocks) 
{

    StockPrice *prices = stocks->elements;
    float max_price    = prices[0].price;
    for (size_t i = 1; i < stocks->num_elements;i++) {

        if (prices[i].price > max_price)
            max_price = prices[i].price;

    }
    return max_price;  

}

/*
// Ramer–Douglas–Peucker
Vector *Graph_RDPAlgorithm(Vector *stocks, float epsilon) 
{

    float dmax = 0.0;
    size_t idx = 0;
    size_t end = stocks->num_elements;

    float d = 0.0;
    for (size_t i = 1; i < end-1;i++) {

        if (d > dmax) {

            idx  = i;
            dmax = d;

        }

    }

}
*/

void Graph_SetGraphPoints(DrawObject *object, Vector *stocks) 
{

    float point_width_diff   = (float)object->width / stocks->num_elements;
    float min_price          = Graph_GetMinPrice(stocks);
    float max_min_price_diff = Graph_GetMaxPrice(stocks) - min_price;

    StockPrice *prices = stocks->elements;
    for (unsigned int i = 0; i < stocks->num_elements;i++) {

        object->graph.points[i].x = point_width_diff*i;
        object->graph.points[i].y = ((prices[i].price - min_price)/(max_min_price_diff))*object->height;

    }

}

DrawObject *Graph_ConstructGraphDrawObject(char *company_name, int timespan_index, int width, int height) 
{

    Vector *stocks = Simulation_GetStockPrices(company_name, Game_GetGameTime(), timespans[timespan_index].diff);
    if (stocks == NULL)
        return NULL;

    //if (stocks->num_elements > 500)
    //    stocks = Graph_RDPAlgorithm(stocks, epsilon);

    DrawObject *object = GetBasicGraphDrawObject(width, height, stocks->num_elements);
    Graph_SetGraphPoints(object, stocks);
    Vector_Delete(stocks);

    return object;

}

DrawObject *Graph_GetGraphDrawObject(char *company_name, TimeSpan timespan, int width, int height) 
{

    DrawObject *graph_object = Graph_ConstructGraphDrawObject(company_name, (int)timespan, width, height);

    if (graph_object != NULL) {

        graph_object->graph.company  = company_name;
        graph_object->graph.timespan = timespan;

    }

    return graph_object;

}

DrawObject *Graph_PollForNewGraphObject(DrawObject *object) 
{

    if (IsTimeSpecInPast(&object->graph.next_refresh)) {

        free(object->graph.points);
        DrawObject *graph_object   = Graph_GetGraphDrawObject(object->graph.company, object->graph.timespan, object->width, object->height);
        object->graph              = graph_object->graph;
        object->graph.next_refresh = GetOffsetTime(75);
        free(graph_object);

    }
    return object;

}

void DrawGraph(DrawObject *object) 
{

    object = Graph_PollForNewGraphObject(object);
    Point *points = object->graph.points;

    if (points == NULL || object->graph.num_points == 0)
        return;

    const float x             = object->x;
    const float y_start_point = object->y + object->height;
    ALLEGRO_COLOR color       = al_map_rgba(255, 255, 255, 255);
    for (unsigned short int i = 0;i < object->graph.num_points - 1;i++)
        al_draw_line(x + points[i].x, y_start_point - points[i].y, x + points[i+1].x, y_start_point - points[i+1].y, color , 2);

}
