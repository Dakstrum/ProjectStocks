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
static MenuWithChilds *news_menu = NULL;

void InitializeGeneralPurposeMenu() 
{ 
    if (CreateNewDrawLayer() == -1) {

        Log("STUB: GeneralPurposeMenu could not create new draw layer");
        return;
    }


    news_menu = GetMenuWithChildsFromJsonLayer("NewsMenu");
    AddMenuWithChildsToDrawLayer(news_menu);

}


void RenderGeneralPurposeMenu() 
{
    DrawLayers();
}

void CleanUpGeneralPurposeMenus() 
{
    free(news_menu);
}

