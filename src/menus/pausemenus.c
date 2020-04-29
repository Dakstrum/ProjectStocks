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
#include "rendering.h"
#include "account.h"
#include "button.h"
#include "scrollbox.h"
#include "optionsmenu.h"

static MenuWithChilds *in_game_pause_menu = NULL;

void ToggleInGamePauseMenu()
{

    if (in_game_pause_menu == NULL) {

        CreateNewDrawLayer();
        in_game_pause_menu = GetMenuWithChildsFromJsonLayer("InGamePauseMenu");
        AddMenuWithChildsToDrawLayer(in_game_pause_menu);
        
    } else {

        ClearCurrentDrawLayer();
        in_game_pause_menu = NULL;

    }

}

void PauseMenuMainMenu_BCB()
{

    ClearDrawLayers();
    SwitchToRenderingMainMenu();

}

void PauseMenuOptions_BCB()
{

    ToggleOptionsMenu();

}

void PauseMenuExit_BCB()
{

    SetCleanUpToTrue();

}