#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"

static MenuWithChilds *news_menu = NULL;

void InitializeNewsMenu() 
{ 

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: StocksMenu could not create new draw layer");
        return;

    }

    news_menu = GetMenuWithChildsFromJsonLayer("NewsMenu");
    AddMenuWithChildsToDrawLayer(news_menu);

}


void CleanUpNewsMenu() 
{

    free(news_menu);

}

