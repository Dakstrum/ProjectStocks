
#include "log.h"
#include "drawlayers.h"
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

MenuWithChilds *GetMenuWithChildsFromJsonLayer(char *menu_name) 
{

    MenuWithChilds *menu_with_childs = GetMenuWithChildsFromDrawObjectJson(menu_name);

    if (menu_with_childs == NULL)
        LogF("Could not find any menu_with_childs %s", menu_name);

    return menu_with_childs;

}