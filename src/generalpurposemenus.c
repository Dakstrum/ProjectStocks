#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"

static MenuWithChilds *pause_menu   = NULL;
static MenuWithChilds *options_menu = NULL;

void RenderGeneralPurposeMenus() 
{
    DrawLayers();
}

void CleanUpGeneralPurposeMenus() 
{
    free(pause_menu);
    free(options_menu);
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

void TogglePauseMenu()
{

    if (pause_menu == NULL) {

        CreateNewDrawLayer();
        pause_menu = GetMenuWithChildsFromJsonLayer("PauseMenu");
        AddMenuWithChildsToDrawLayer(pause_menu);
        
    } else {

        ClearCurrentDrawLayer();
        pause_menu = NULL;

    }

}

