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
static MenuWithChilds *account_menu = NULL;

void InitializeAccountMenu() 
{ 
    if (CreateNewDrawLayer() == -1) {

        Log("STUB: account Menu could not create new draw layer");
        return;
    }


    account_menu = GetMenuWithChildsFromJsonLayer("AccountMenu");
    AddMenuWithChildsToDrawLayer(account_menu);

    Log("Created Main Menu Draw Layer");
    LogF("Num_text_found = %d", account_menu->num_text);

}


void RenderAccountMenu() 
{
    DrawLayers();
}

void CleanUpAccountMenu() 
{
    free(account_menu);
}

