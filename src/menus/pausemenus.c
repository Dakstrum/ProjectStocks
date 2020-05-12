#include <stdio.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"
#include "rendering.h"
#include "account.h"
#include "optionsmenu.h"
#include "dbaccount.h"

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

    SetDBMoneyToLocalMoney(GetCurrentPlayerId());
    ClearDrawLayers();
    SwitchToRenderingMainMenu();

}

void PauseMenuOptions_BCB()
{

    ToggleOptionsMenu();

}

void PauseMenuSave_BCB()
{

    Log("saving from pause menu button :))");
    
}


void PauseMenuExit_BCB()
{

    SetDBMoneyToLocalMoney(GetCurrentPlayerId());
    SetCleanUpToTrue();

}