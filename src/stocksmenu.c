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
    DrawObject *popup_object = CreateNewDrawObject();

    popup_object->type = POPUP;
    popup_object->should_this_be_drawn = true;
    popup_object->x = 0;
    popup_object->y = 0;

    popup_object->width  = 1920;
    popup_object->height = 1080;
    
    popup_object->asset_path = "assets/images/generalpurposemenus/popups/genericpopup.png";
    popup_object->member.popup.current_time = 1;
    popup_object->member.popup.end_time     = 180;
    AddObjectToDrawLayer(popup_object);
}


void RenderStocksMenu() 
{

    DrawLayers();

}

void CleanUpStocksMenu() 
{

    free(stocks_menu);
    
}

