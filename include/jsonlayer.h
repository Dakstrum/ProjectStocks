#ifndef JSONLAYER_H
#define JSONLAYER_H

#include "drawlayers.h"

void InitializeJson();
void CleanUpJson();

DrawObject *GetDrawObjectFromJsonLayer(char *object_name);

#endif