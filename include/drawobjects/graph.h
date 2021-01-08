#ifndef GRAPH_H
#define GRAPH_H

#include "drawobject.h"
#include "shared.h"

DrawObject *Graph_GetGraphDrawObject(char *company_name, TimeSpan timespan, int width, int height);
void Graph_Draw(DrawObject *object);

void Graph_Clean(DrawObject *object);

#endif