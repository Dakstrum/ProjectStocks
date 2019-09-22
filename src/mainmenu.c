#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"
#include "stocksmenu.h"

//static DrawObject *main_menu     = NULL;
//static DrawObject *version_text  = NULL;
static MenuWithChilds *main_menu    = NULL;
static MenuWithChilds *options_menu = NULL;

void InitializeMainMenu() 
{

    
    if (CreateNewDrawLayer() == -1) {

        Log("ERROR: MainMenu could not create new draw layer");
        return;

    }
    
    main_menu = GetMenuWithChildsFromJsonLayer("MainMenu");
    AddMenuWithChildsToDrawLayer(main_menu);

}


void RenderMainMenu() 
{
    DrawLayers();
}

void CleanUpMainMenu() 
{
    free(main_menu);
}

void InitStartGame()
{
    InitializeStocksMenu();
}

void ToggleOptionsMenu()
{

    if (options_menu == NULL) {

        CreateNewDrawLayer();
        options_menu = GetMenuWithChildsFromJsonLayer("OptionsMenu");
        AddMenuWithChildsToDrawLayer(options_menu);
        
    } else {

        ClearCurrentDrawLayer();
        options_menu = NULL;

    }

}