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
#include "optionsmenu.h"
#include "savemenus.h"
#include "pausemenus.h"

#include "rendering.h"
#include "account.h"
#include "game.h"
#include "button.h"
#include "scrollbox.h"

#include "drawlayerutils.h"

void GeneralPurposeMenus_SetSpeedOne_CB();
void GeneralPurposeMenus_SetSpeedTwo_CB();
void GeneralPurposeMenus_SetSpeedThree_CB();

static DrawObject *speed_one_button   = NULL;
static DrawObject *speed_two_button   = NULL;
static DrawObject *speed_three_button = NULL;

char* GeneralPurposeMenus_GetSpeedButtonName(char* menu_name, char* button_object_name);

static int account_timespeed          = 1;

void GeneralPurposeMenus_InitSpeedSelectObject(char* menu_name)
{

    speed_one_button   = GetDrawObjectByName(GeneralPurposeMenus_GetSpeedButtonName(menu_name, "Speed1ButtonObject"));
    speed_two_button   = GetDrawObjectByName(GeneralPurposeMenus_GetSpeedButtonName(menu_name, "Speed2ButtonObject"));
    speed_three_button = GetDrawObjectByName(GeneralPurposeMenus_GetSpeedButtonName(menu_name, "Speed3ButtonObject"));

    if(account_timespeed == 1) GeneralPurposeMenus_SetSpeedOne_CB();
    if(account_timespeed == 2) GeneralPurposeMenus_SetSpeedTwo_CB();
    if(account_timespeed == 3) GeneralPurposeMenus_SetSpeedThree_CB();

}

char* GeneralPurposeMenus_GetSpeedButtonName(char* menu_name, char* button_object_name)
{

    static char speed_temp_drawobject[128] = {'\0'};
    strcpy(speed_temp_drawobject, menu_name);
    strcat(speed_temp_drawobject, button_object_name);
    speed_temp_drawobject[127] = '\0';
    return speed_temp_drawobject;

}

void GeneralPurposeMenus_InitStocksMenu_CB()
{

    ClearCurrentDrawLayer();
    SwitchToRenderingStocksMenu();

}

void GeneralPurposeMenus_InitNewsMenu_CB()
{

    ClearCurrentDrawLayer();
    SwitchToRenderingNewsMenu();
    
}

void GeneralPurposeMenus_InitAccountMenu_CB()
{
    
    ClearCurrentDrawLayer();
    SwitchToRenderingAccountMenu();
    
}

void GeneralPurposeMenus_InitCardsMenu_CB()
{
    
    ClearCurrentDrawLayer();
    SwitchToRenderingCardsMenu();
    
}

void GeneralPurposeMenus_SetSpeedOne_CB()
{

    SetButtonTint(speed_one_button,   GetRGBA(0, 255, 0, 200));
    SetButtonTint(speed_two_button,   GetRGBA(38, 50, 56, 180));
    SetButtonTint(speed_three_button, GetRGBA(38, 50, 56, 180));
    Game_SetSpeed(1);
    account_timespeed = 1;
    
}
void GeneralPurposeMenus_SetSpeedTwo_CB()
{

    SetButtonTint(speed_one_button,   GetRGBA(0, 255, 0, 200));
    SetButtonTint(speed_two_button,   GetRGBA(0, 255, 0, 200));
    SetButtonTint(speed_three_button, GetRGBA(38, 50, 56, 180));
    Game_SetSpeed(2);
    account_timespeed = 2;

}
void GeneralPurposeMenus_SetSpeedThree_CB()
{

    SetButtonTint(speed_one_button,   GetRGBA(0, 255, 0, 200));
    SetButtonTint(speed_two_button,   GetRGBA(0, 255, 0, 200));
    SetButtonTint(speed_three_button, GetRGBA(0, 255, 0, 200));
    Game_SetSpeed(3);
    account_timespeed = 3;

}

void GeneralPurposeMenus_CleanSpeedButtons()
{

    speed_one_button   = NULL;
    speed_two_button   = NULL;
    speed_three_button = NULL;

}

void GeneralPurposeMenus_CleanAllMenus() 
{

    AccountMenu_Clear();
    CleanSaveMenu();
    CardsMenu_Clean();
    GeneralPurposeMenus_CleanSpeedButtons();
    OptionsMenu_Clean();
    PauseMenu_Clean();
    CleanStocksMenu();

}