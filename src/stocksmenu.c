#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"
#include "cache.h"


static MenuWithChilds *stocks_menu = NULL;

void DisplayTempPopUp();

void InitializeStocksMenu() 
{ 
    

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: StocksMenu could not create new draw layer");
        return;
    }


    stocks_menu = GetMenuWithChildsFromJsonLayer("StocksMenu");
    AddMenuWithChildsToDrawLayer(stocks_menu);
    DisplayTempPopUp();

}

void DisplayTempPopUp()
{
    static DrawObject *object = CreateNewDrawObject();
    object->type = POPUP;
    object->should_this_be_drawn = true;
    object->x = 0;
    object->y = 0;

    object->width  = 1920;
    object->height = 1080;
    
    object->asset_path = "assets/images/generalpurposemenus/popups/genericpopup.png";
    object->member.popup.current_time = 1;
    object->member.popup.end_time     = 180;
    //AddObjectToDrawLayer(object);
}


void RenderStocksMenu() 
{

    DrawLayers();

}

void CleanUpStocksMenu() 
{

    free(stocks_menu);
    
}

