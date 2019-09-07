
#include "jsonlayer.h"
#include "jsoncompanies.h"
#include "jsondrawobjects.h"

void InitializeJson()
{

    InitializeCompaniesJson();
    InitialzeDrawObjectsJson();

}

void CleanUpJson() 
{

    CleanUpDrawObjectJson();
    CleanUpComponaniesJson();

}

DrawObject *GetDrawObjectFromJsonLayer(char *object_name) 
{

    return GetDrawObjectFromDrawObjectJson(object_name);

}