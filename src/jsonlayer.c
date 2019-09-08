
#include "log.h"
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

    DrawObject *object = GetDrawObjectFromDrawObjectJson(object_name);

    if (object == NULL)
        LogF("Could not find object with name %s", object_name);

    return object;

}