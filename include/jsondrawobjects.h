#ifndef JSONDRAWOBJECTS_H
#define JSONDRAWOBJECTS_H

#include "drawlayers.h"

void InitialzeDrawObjectsJson();
DrawObject *GetDrawObjectFromDrawObjectJson(char *object_name);
void CleanUpDrawObjectJson();

#endif