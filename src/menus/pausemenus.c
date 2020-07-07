#include <stdio.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"
#include "rendering.h"
#include "account.h"
#include "optionsmenu.h"
#include "dbaccount.h"
#include "drawlayerutils.h"

static MenuWithChilds *in_game_pause_menu = NULL;

void ToggleInGamePauseMenu()
{

    if (in_game_pause_menu == NULL) {

        CreateNewDrawLayer();
        in_game_pause_menu = GetJSONMenuAndAddToDrawLayer("PauseMenu");
        
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

void PauseMenuSave_BCB()
{

    StorePlayerData();
    
}


void PauseMenuExit_BCB()
{

    //SetDBMoneyToLocalMoney(GetCurrentPlayerId());
    SetCleanUpToTrue();

}

void CleanPauseMenu()
{
    in_game_pause_menu = NULL;
}