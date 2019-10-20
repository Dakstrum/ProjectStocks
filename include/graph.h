#ifndef GRAPH_H
#define GRAPH_H

#include "drawlayers.h"
#include "shared.h"

void InitializeGraphCaching();
DrawObject *GetGraphDrawObject(char *company_name, TimeSpan timespan, int width, int height);
void CleanUpGraph();

#endif