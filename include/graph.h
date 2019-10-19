#ifndef GRAPH_H
#define GRAPH_H

#include "drawlayers.h"

typedef enum TimeSpan {

    ONE_DAY = 0,
    THREE_DAYS,
    ONE_WEEK,
    TWO_WEEKS,
    ONE_MONTH,
    THREE_MONTHS,
    SIX_MONTHS,
    ONE_YEAR,
    TWO_YEARS,
    FIVE_YEARS,
    ALL_TIME

} TimeSpan;

void InitializeGraphCaching();
DrawObject *GetGraphDrawObject(char *company_name, TimeSpan timespan, int width, int height);

#endif