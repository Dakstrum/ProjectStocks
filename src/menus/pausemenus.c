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

void PauseMenu_Init()
{

    CreateNewDrawLayer();
    in_game_pause_menu = GetJSONMenuAndAddToDrawLayer("PauseMenu");

}

void PauseMenu_Toggle()
{

    if (in_game_pause_menu == NULL) {

        PauseMenu_Init();
        Timer_Pause();
        
    } else {

        ClearCurrentDrawLayer();
        in_game_pause_menu = NULL;
        Timer_Unpause();

    }

}

void PauseMenu_MainMenu_CB()
{

    Timer_Unpause();
    ClearDrawLayers();
    SwitchToRenderingMainMenu();

}

void PauseMenu_Options_CB()
{

    ClearCurrentDrawLayer();
    in_game_pause_menu = NULL;
    OptionsMenu_Toggle();

}

void PauseMenu_Save_CB()
{

    Account_StorePlayerData();
    
}

void PauseMenu_Exit_CB()
{

    //SetDBMoneyToLocalMoney(Account_GetPlayerId());
    SetCleanUpToTrue();

}

void PauseMenu_Clean()
{

    in_game_pause_menu = NULL;
    
}