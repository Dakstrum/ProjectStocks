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
#include "cardsmenu.h"
#include "mainmenu.h"

static MenuWithChilds *pause_menu   = NULL;
static MenuWithChilds *options_menu = NULL;

char MenusThatDisablePauseMenuFunctionality[] = 
{
    "OptionsMenu"
};

void CleanUpGeneralPurposeMenus() 
{

    pause_menu   = NULL;
    options_menu = NULL;

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

void PauseMenuMainMenu_BCB()
{

    pause_menu = NULL;
    ClearDrawLayers();
    InitializeMainMenu();

}

void PauseMenuOptions_BCB()
{

    ToggleOptionsMenu();

}

void PauseMenuExit_BCB()
{

    SetCleanUpToTrue();

}

void OptionsMenuExit_BCB()
{

    ToggleOptionsMenu();

}


void Stocks_BCB()
{

    ClearCurrentDrawLayer();
    InitializeStocksMenu();

}

void News_BCB()
{
    
    ClearCurrentDrawLayer();
    InitializeNewsMenu();
    
}

void Account_BCB()
{
    
    ClearCurrentDrawLayer();
    InitializeAccountMenu();
    
}

void Cards_BCB()
{
    
    ClearCurrentDrawLayer();
    InitializeCardsMenu();
    
}