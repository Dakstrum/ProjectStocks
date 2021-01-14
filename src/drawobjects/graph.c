
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

DrawObject *Graph_GetDrawObject(int width, int height, int num_points) 
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
    object->graph.stock_prices = Vector_Create(sizeof(StockPrice), 512);
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

void Graph_DistanceReduction(DrawObject *object) 
{

    Vector *points               = object->graph.points;
    Vector *new_point_vec        = Vector_Create(sizeof(Point), 512);
    Vector *new_stock_price_vec  = Vector_Create(sizeof(StockPrice), 512);
    Point *new_point_vec_temp    = new_point_vec->elements;
    StockPrice *stock_prices_arr = ((Vector *)object->graph.stock_prices)->elements;

    Point *vector_points = points->elements; 
    Vector_PushBack(new_point_vec, &vector_points[0]);
    Vector_PushBack(new_stock_price_vec, &stock_prices_arr[0]);


    float dx = 0.0;
    float dy = 0.0;
    float d  = 0.0;
    for (size_t i = 1; i < points->num_elements;i++) {

        dx = (float)new_point_vec_temp[new_point_vec->num_elements - 1].x - (float)vector_points[i].x;
        dy = (float)new_point_vec_temp[new_point_vec->num_elements - 1].y - (float)vector_points[i].y;
        d  = sqrt(dx * dx + dy * dy);

        if (d > 7.5) {

            Vector_PushBack(new_point_vec, &vector_points[i]);
            Vector_PushBack(new_stock_price_vec, &stock_prices_arr[i]);

        }

    }

    Vector_Delete(object->graph.points);
    Vector_Delete(object->graph.stock_prices);
    object->graph.points       = new_point_vec;
    object->graph.stock_prices = new_stock_price_vec;

}

void Graph_ReducePoints(DrawObject *object) 
{

    if (object->graph.points->num_elements < 500)
        return;

    Graph_DistanceReduction(object);

}


void Graph_SetGraphPoints(DrawObject *object, Vector *stocks) 
{

    float point_width_diff   = (float)object->width / stocks->num_elements;
    float min_price          = Graph_GetMinPrice(stocks);
    float max_min_price_diff = Graph_GetMaxPrice(stocks) - min_price;

    StockPrice *prices = stocks->elements;

    Point point;
    StockPrice stock_price;
    for (unsigned int i = 0; i < stocks->num_elements;i++) {

        point.x = point_width_diff*i;
        point.y = ((prices[i].price - min_price)/(max_min_price_diff))*object->height;
        stock_price.price = prices[i].price;
        stock_price.date  = prices[i].date;
        Vector_PushBack(object->graph.points, &point);
        Vector_PushBack(object->graph.stock_prices, &stock_price);

    }

}

DrawObject *Graph_ConstructGraphDrawObject(char *company_name, int timespan_index, int width, int height) 
{

    Vector *stocks = Simulation_GetStockPrices(company_name, Game_GetGameTime(), timespans[timespan_index].diff);
    if (stocks == NULL)
        return NULL;

    DrawObject *object = Graph_GetDrawObject(width, height, stocks->num_elements);
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

        float m_x = object->graph.m_x;
        float m_y = object->graph.m_y;

        Vector_Delete(object->graph.points);
        Vector_Delete(object->graph.stock_prices);
        DrawObject *graph_object   = Graph_GetGraphDrawObject(object->graph.company, object->graph.timespan, object->width, object->height);
        object->graph              = graph_object->graph;
        object->graph.next_refresh = GetOffsetTime(75);
        free(graph_object);

        object->graph.m_x = m_x;
        object->graph.m_y = m_y;

    }
    return object;

}

uint16_t Graph_GetClosestPointByDx(DrawObject *object) 
{

    const float x = object->x;
    Point *points = object->graph.points->elements;

    size_t selected_idx = 0;
    float dx = fabs(x + points[0].x - object->graph.m_x);
    for (size_t i = 1; i < object->graph.points->num_elements;i++) {

        float temp = fabs(x + points[i].x - object->graph.m_x);
        if (temp < dx) {

            dx = temp;
            selected_idx = i;

        }

    }

    return selected_idx;

}

void Graph_DrawTextOverlay(DrawObject *object)
{

    const float x = object->x;
    const float y_start_point = object->y + object->height;
    uint16_t selected_idx = Graph_GetClosestPointByDx(object);

    Point *points            = object->graph.points->elements;
    StockPrice *stock_prices = ((Vector *)object->graph.stock_prices)->elements;

    if (fabs(x + points[selected_idx].x - object->graph.m_x) > 10.0)
        return;

    char time_buff[32];
    char buff[64];
    strftime(time_buff, 32, "%HH %x", localtime(&stock_prices[selected_idx].date));
    sprintf(buff, "$%.2f, %s", stock_prices[selected_idx].price, time_buff);

    ALLEGRO_FONT *font = GetFontFromCache("assets/font/DanielLinssenM5/m5x7.ttf", 40);
    ALLEGRO_COLOR color = al_map_rgba(255, 255, 255, 255);
    al_draw_text(font, color, object->graph.m_x + 15, object->graph.m_y, 0, buff);
    al_draw_line(x + points[selected_idx].x, y_start_point - points[selected_idx].y, x + points[selected_idx].x, object->graph.m_y, color, 2);  

}

void Graph_Draw(DrawObject *object) 
{

    object = Graph_PollForNewGraphObject(object);

    if (object->graph.points->num_elements == 0)
        return;

    const float x              = object->x;
    const float y_start_point  = object->y + object->height;
    ALLEGRO_COLOR color = al_map_rgba(255, 255, 255, 255);

    Point *points = object->graph.points->elements;
    for (size_t i = 0;i < object->graph.points->num_elements-1;i++)
        al_draw_line(x + points[i].x, y_start_point - points[i].y, x + points[i+1].x, y_start_point - points[i+1].y, color , 2);

    if (object->graph.m_x != -1.0)
        Graph_DrawTextOverlay(object);

}

void Graph_Clean(DrawObject *object) 
{

    Vector_Delete(object->graph.points);
    Vector_Delete(object->graph.stock_prices);

}