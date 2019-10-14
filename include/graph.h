#ifndef GRAPH_H
#define GRAPH_H

#include "drawlayers.h"

void InitializeGraphCaching();
DrawObject *GetGraphDrawObject(char *company_name, char *timespan, int width, int height);

#endif