
#include <time.h>
#include <math.h>
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
#include "vector.h"

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
    object->graph.points       = Vector_Create(sizeof(Point), 512);

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

float Graph_PointLineDistance(Point point, Point p_1, Point p_2) 
{

    float dy = p_2.y - p_1.y;
    float dx = p_2.x - p_1.x;

    float num = fabs(dy * point.x + dx * point.y);
    float dom = sqrt(dy * dy + dx * dx);

    return num/dom;

}

Vector *Graph_DistanceReduction(Vector *points) 
{

    Vector *temp = Vector_Create(sizeof(Point), 512);
    Point *temp_points = temp->elements;

    Point *vector_points = points->elements; 
    Vector_PushBack(temp, &vector_points[0]);


    float dx = 0.0;
    float dy = 0.0;
    float d  = 0.0;
    for (size_t i = 1; i < points->num_elements;i++) {

        dx = temp_points[temp->num_elements - 1].x - vector_points[i].x;
        dy = temp_points[temp->num_elements - 1].y - vector_points[i].y;

        d = sqrt(dx * dx + dy * dy);

        if (d > 7.5) {

            Vector_PushBack(temp, &vector_points[i]);

        }

    }

    return temp;

}

void Graph_ReducePoints(DrawObject *object) 
{

    if (object->graph.points->num_elements < 500)
        return;

    Vector *new_points = Graph_DistanceReduction(object->graph.points);
    Vector_Delete(object->graph.points);
    object->graph.points = new_points;

}


void Graph_SetGraphPoints(DrawObject *object, Vector *stocks) 
{

    float point_width_diff   = (float)object->width / stocks->num_elements;
    float min_price          = Graph_GetMinPrice(stocks);
    float max_min_price_diff = Graph_GetMaxPrice(stocks) - min_price;

    StockPrice *prices = stocks->elements;

    Point point;
    for (unsigned int i = 0; i < stocks->num_elements;i++) {

        point.x = point_width_diff*i;
        point.y = ((prices[i].price - min_price)/(max_min_price_diff))*object->height;
        Vector_PushBack(object->graph.points, &point);

    }

}

DrawObject *Graph_ConstructGraphDrawObject(char *company_name, int timespan_index, int width, int height) 
{

    Vector *stocks = Simulation_GetStockPrices(company_name, Game_GetGameTime(), timespans[timespan_index].diff);
    if (stocks == NULL)
        return NULL;

    DrawObject *object = GetBasicGraphDrawObject(width, height, stocks->num_elements);
    Graph_SetGraphPoints(object, stocks);
    Graph_ReducePoints(object);
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

        Vector_Delete(object->graph.points);
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

    if (object->graph.points->num_elements == 0)
        return;

    const float x             = object->x;
    const float y_start_point = object->y + object->height;
    ALLEGRO_COLOR color       = al_map_rgba(255, 255, 255, 255);

    Point *points = object->graph.points->elements;
    for (size_t i = 0;i < object->graph.points->num_elements-1;i++)
        al_draw_line(x + points[i].x, y_start_point - points[i].y, x + points[i+1].x, y_start_point - points[i+1].y, color , 2);

}
