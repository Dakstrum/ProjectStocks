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

void SpeedOne_BCB();
void SpeedTwo_BCB();
void SpeedThree_BCB();

static MenuWithChilds *pause_menu     = NULL;
static MenuWithChilds *options_menu   = NULL;

static DrawObject *pause_button       = NULL;
static DrawObject *speed_one_button   = NULL;
static DrawObject *speed_two_button   = NULL;
static DrawObject *speed_three_button = NULL;

static int account_timespeed          = 1;

void CleanUpGeneralPurposeMenus() 
{ 

    pause_menu   = NULL;
    options_menu = NULL;

}
  
void InitializeDynamicObjects()
{

    pause_button       = GetDrawObjectByName("PauseSpeedButton");
    speed_one_button   = GetDrawObjectByName("SpeedOneButton");
    speed_two_button   = GetDrawObjectByName("SpeedTwoButton");
    speed_three_button = GetDrawObjectByName("SpeedThreeButton");

    if (pause_button == NULL)
        return;

    if(account_timespeed == 1)
    {
        SpeedOne_BCB();
    }
    if(account_timespeed == 2)
    {
        SpeedTwo_BCB();
    }
    if(account_timespeed == 3)
    {
        SpeedThree_BCB();
    }
    
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

void OptionsMenuExit_BCB()
{

    ToggleOptionsMenu();

}

void Stocks_BCB()
{

    ClearCurrentDrawLayer();
    SwitchToRenderingStocksMenu();

}

void News_BCB()
{
    
    ClearCurrentDrawLayer();
    SwitchToRenderingNewsMenu();
    
}

void Account_BCB()
{
    
    ClearCurrentDrawLayer();
    SwitchToRenderingAccountMenu();
    
}

void Cards_BCB()
{
    
    ClearCurrentDrawLayer();
    SwitchToRenderingCardsMenu();
    
}

void PauseSpeed_BCB()
{

    SetButtonTint(pause_button,       GetRGBA(255, 0, 0, 200));
    SetButtonTint(speed_one_button,   GetRGBA(38, 50, 56, 180));
    SetButtonTint(speed_two_button,   GetRGBA(38, 50, 56, 180));
    SetButtonTint(speed_three_button, GetRGBA(38, 50, 56, 180));

}
void SpeedOne_BCB()
{

    SetButtonTint(pause_button,       GetRGBA(38, 50, 56, 180));
    SetButtonTint(speed_one_button,   GetRGBA(0, 255, 0, 200));
    SetButtonTint(speed_two_button,   GetRGBA(38, 50, 56, 180));
    SetButtonTint(speed_three_button, GetRGBA(38, 50, 56, 180));
    SetGameSpeed(1);
    account_timespeed = 1;
    

}
void SpeedTwo_BCB()
{

    SetButtonTint(pause_button,       GetRGBA(38, 50, 56, 180));
    SetButtonTint(speed_one_button,   GetRGBA(0, 255, 0, 200));
    SetButtonTint(speed_two_button,   GetRGBA(0, 255, 0, 200));
    SetButtonTint(speed_three_button, GetRGBA(38, 50, 56, 180));
    SetGameSpeed(2);
    account_timespeed = 2;

}
void SpeedThree_BCB()
{

    SetButtonTint(pause_button,       GetRGBA(38, 50, 56, 180));
    SetButtonTint(speed_one_button,   GetRGBA(0, 255, 0, 200));
    SetButtonTint(speed_two_button,   GetRGBA(0, 255, 0, 200));
    SetButtonTint(speed_three_button, GetRGBA(0, 255, 0, 200));
    SetGameSpeed(3);
    account_timespeed = 3;

}