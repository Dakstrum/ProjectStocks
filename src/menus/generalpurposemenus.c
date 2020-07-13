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
#include "drawlayerutils.h"

void SpeedOne_BCB();
void SpeedTwo_BCB();
void SpeedThree_BCB();

static DrawObject *pause_button       = NULL;
static DrawObject *speed_one_button   = NULL;
static DrawObject *speed_two_button   = NULL;
static DrawObject *speed_three_button = NULL;

static int account_timespeed          = 1;

void InitializeSpeedSelectObject(char* menu_name)
{

    char speed_one_drawobject[128] = {'\0'};
    strcpy(speed_one_drawobject, menu_name);
    strcat(speed_one_drawobject, "Speed1ButtonObject");
    speed_one_drawobject[127] = '\0';
    speed_one_button   = GetDrawObjectByName(speed_one_drawobject);

    char speed_two_drawobject[128] = {'\0'};
    strcpy(speed_two_drawobject, menu_name);
    strcat(speed_two_drawobject, "Speed2ButtonObject");
    speed_two_drawobject[127] = '\0';
    speed_two_button   = GetDrawObjectByName(speed_two_drawobject);

    char speed_three_drawobject[128] = {'\0'};
    strcpy(speed_three_drawobject, menu_name);
    strcat(speed_three_drawobject, "Speed3ButtonObject");
    speed_three_drawobject[127] = '\0';
    speed_three_button   = GetDrawObjectByName(speed_three_drawobject);

    if(account_timespeed == 1) SpeedOne_BCB();
    if(account_timespeed == 2) SpeedTwo_BCB();
    if(account_timespeed == 3) SpeedThree_BCB();

    if (pause_button == NULL)
        return;


    
}

void CleanAll() 
{

    ClearDrawLayers();


    CleanAccountMenu();
    CleanCardsMenu();
    CleanOptionsMenu();
    CleanStocksMenu();

}

void Stocks_BCB()
{

    CleanAll();
    SwitchToRenderingStocksMenu();

}

void News_BCB()
{

    CleanAll();
    SwitchToRenderingNewsMenu();
    
}

void Account_BCB()
{
    
    CleanAll();
    SwitchToRenderingAccountMenu();
    
}

void Cards_BCB()
{
    
    CleanAll();
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

void CleanGeneralPurposeMenu()
{

    pause_button       = NULL;
    speed_one_button   = NULL;
    speed_two_button   = NULL;
    speed_three_button = NULL;

}