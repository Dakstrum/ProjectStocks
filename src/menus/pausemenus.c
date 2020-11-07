#include <stdio.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"
#include "rendering.h"
#include "account.h"
#include "optionsmenu.h"
#include "dbaccount.h"
#include "drawlayerutils.h"
#include "dbcard.h"

#include "timer.h"

static MenuWithChilds *in_game_pause_menu = NULL;

void CreateInGamePauseMenu()
{

    CreateNewDrawLayer();
    in_game_pause_menu = GetJSONMenuAndAddToDrawLayer("PauseMenu");

}

void ToggleInGamePauseMenu()
{

    if (in_game_pause_menu == NULL) {

        CreateInGamePauseMenu();
        Timer_Pause();
        
    } else {

        ClearCurrentDrawLayer();
        in_game_pause_menu = NULL;
        Timer_Unpause();

    }

}

void PauseMenuMainMenu_BCB()
{

    Timer_Unpause();
    ClearDrawLayers();
    SwitchToRenderingMainMenu();

}

void PauseMenuOptions_BCB()
{

    ClearCurrentDrawLayer();
    in_game_pause_menu = NULL;
    ToggleOptionsMenu();

}

void PauseMenuSave_BCB()
{

    StorePlayerData();
    SaveCards();
    
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