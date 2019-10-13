#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"
#include "cache.h"

//static DrawObject *stocks_menu     = NULL;
//static DrawObject *version_text  = NULL;
static MenuWithChilds *stocks_menu = NULL;

void InitializeStocksMenu() 
{ 

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: StocksMenu could not create new draw layer");
        return;
    }

    //temp
    DrawObject *object = CreateNewDrawObject();
    object->type = POPUP;
    object->should_this_be_drawn = true;
    object->x = 0;
    object->y = 0;

    object->width = 400;
    object->height = 400;
    
    object->asset_path = "assets/images/stocksmenu/stocksmenu.png";
    object->member.popup.current_time = 1;
    object->member.popup.end_time = 180;

    stocks_menu = GetMenuWithChildsFromJsonLayer("StocksMenu");

    AddMenuWithChildsToDrawLayer(stocks_menu);
    AddObjectToDrawLayer(object);
    //end temp

}


void RenderStocksMenu() 
{

    DrawLayers();

}

void CleanUpStocksMenu() 
{

    free(stocks_menu);
    
}

