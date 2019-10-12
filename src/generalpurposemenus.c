#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"
#include "shared.h"
#include "stocksmenu.h"
#include "newsmenu.h"
#include "accountmenu.h"

static MenuWithChilds *pause_menu   = NULL;
static MenuWithChilds *options_menu = NULL;

char MenusThatDisablePauseMenuFunctionality[] = 
{
    "OptionsMenu"
};

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
        options_menu = GetMenuWithChildsFromJsonLayer(MenusThatDisablePauseMenuFunctionality);
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

//General Purpose Menu Callbacks
void PauseMenuOptionsButtonCallBack()
{

    ToggleOptionsMenu();

}

void PauseMenuExitButtonCallBack()
{

    SetCleanUpToTrue();

}

void OptionsMenuExitButtonCallBack()
{

    ToggleOptionsMenu();

}

//Navigation Bar Callbacks
void StocksButtonCallBack()
{

    ClearCurrentDrawLayer();
    InitializeStocksMenu();

}

void NewsButtonCallBack()
{
    
    ClearCurrentDrawLayer();
    InitializeNewsMenu();
    
}

void AccountButtonCallBack()
{
    
    ClearCurrentDrawLayer();
    InitializeAccountMenu();
    
}

void EavesdropperButtonCallBack()
{
    
    Log("Eaves");
    
}

