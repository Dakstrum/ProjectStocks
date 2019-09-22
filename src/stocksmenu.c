#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"

//static DrawObject *stocks_menu     = NULL;
//static DrawObject *version_text  = NULL;
static MenuWithChilds *stocks_menu = NULL;

void InitializeStocksMenu() 
{ 

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: StocksMenu could not create new draw layer");
        return;
    }


    stocks_menu = GetMenuWithChildsFromJsonLayer("StocksMenu");
    AddMenuWithChildsToDrawLayer(stocks_menu);

}


void RenderStocksMenu() 
{

    DrawLayers();

}

void CleanUpStocksMenu() 
{

    free(stocks_menu);
    
}

