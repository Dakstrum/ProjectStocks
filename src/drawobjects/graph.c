
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
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
#include "cache.h"

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

typedef struct GraphPoint {

    uint16_t x;
    uint16_t y;
    time_t timestamp;
    float price;

} GraphPoint;

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
    object->graph.points       = Vector_Create(sizeof(GraphPoint), 512);
    object->graph.m_x          = -1.0;
    object->graph.m_y          = -1.0;

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

Vector *Graph_DistanceReduction(Vector *points) 
{

    Vector *temp = Vector_Create(sizeof(GraphPoint), 512);
    GraphPoint *temp_points = temp->elements;

    GraphPoint *vector_points = points->elements; 
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

    GraphPoint point;
    for (unsigned int i = 0; i < stocks->num_elements;i++) {

        point.x = point_width_diff*i;
        point.y = ((prices[i].price - min_price)/(max_min_price_diff))*object->height;
        point.timestamp = prices[i].date;
        point.price = prices[i].price;
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

void Graph_DrawTextOverlay(DrawObject *object)
{

    const float x = object->x;
    const float y_start_point  = object->y + object->height;
    GraphPoint *points = object->graph.points->elements;

    size_t selected_idx = 0;
    float dx = fabs(x + points[0].x - object->graph.m_x);
    for (size_t i = 1; i < object->graph.points->num_elements;i++) {

        float temp = fabs(x + points[i].x - object->graph.m_x);
        if (temp < dx) {

            dx = temp;
            selected_idx = i;

        }

    }

    if (dx > 7.0)
        return;

    char time_buff[32];
    char buff[64];
    strftime(time_buff, 32, "%HH %x", localtime(&points[selected_idx].timestamp));
    sprintf(buff, "$%.2f, %s", points[selected_idx].price, time_buff);

    ALLEGRO_FONT *font = GetFontFromCache("assets/font/DanielLinssenM5/m5x7.ttf", 40);
    ALLEGRO_COLOR color = al_map_rgba(255, 255, 255, 255);
    al_draw_text(font, color, object->graph.m_x, object->graph.m_y, 0, buff);
    al_draw_line(object->graph.m_x, y_start_point - points[selected_idx].y, object->graph.m_x, object->graph.m_y, color, 2);  

}

void DrawGraph(DrawObject *object) 
{

    float m_x = object->graph.m_x;
    float m_y = object->graph.m_y;

    object = Graph_PollForNewGraphObject(object);

    object->graph.m_x = m_x;
    object->graph.m_y = m_y;

    if (object->graph.points->num_elements == 0)
        return;

    const float x              = object->x;
    const float y_start_point  = object->y + object->height;
    ALLEGRO_COLOR color = al_map_rgba(255, 255, 255, 255);

    GraphPoint *points = object->graph.points->elements;
    for (size_t i = 0;i < object->graph.points->num_elements-1;i++)
        al_draw_line(x + points[i].x, y_start_point - points[i].y, x + points[i+1].x, y_start_point - points[i+1].y, color , 2);

    if (object->graph.m_x != -1.0)
        Graph_DrawTextOverlay(object);

}
