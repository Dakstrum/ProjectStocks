#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "log.h"

#include "accountmenu.h"
#include "cardsmenu.h"
#include "generalpurposemenus.h"
#include "newsmenu.h"
#include "optionsmenu.h"
#include "pausemenus.h"
#include "savemenus.h"
#include "stocksmenu.h"
#include "linkopener.h"
#include "account.h"
#include "drawlayerutils.h"

#include "animations.h"

static MenuWithChilds *main_menu = NULL;

void MainMenu_RenderLogic()
{

}

void MainMenu_Init() 
{

    if (CreateNewDrawLayer() == -1) {

        Log("ERROR: MainMenu could not create new draw layer");
        return;

    }

    Account_SetInGameStatus(0);
    
    main_menu = GetJSONMenuAndAddToDrawLayer("MainMenu");
    MainMenu_RenderLogic();
    
    GeneralPurposeMenus_CleanAllMenus();

}


int MainMenu_Active()
{
    if(main_menu)
        return 1;
    else
        return 0;

}

void MainMenu_Start_CB()
{
    
    ClearDrawLayers();
    InitializeLoadSaveMenu();
    main_menu = NULL;
    
}


void MainMenu_Options_CB()
{
    
    OptionsMenu_MainMenuVersonToggle();
    

}

void MainMenu_Exit_CB()
{

    SetCleanUpToTrue();

}

void MainMenu_GitHub_CB()
{

    OpenLink("https://www.github.com");

}

void MainMenu_Twitter_CB()
{

    OpenLink("https://twitter.com/Dakstrum");

}

void MainMenu_Youtube_CB()
{

    OpenLink("https://www.youtube.com/channel/UCIW4bSzn44v08ttyRMT5z2w");

}

void MainMenu_Website_CB()
{

    OpenLink("https://www.dakstrum.com");

}


void MainMenu_Clean() 
{

    if (main_menu != NULL)
        free(main_menu);
    
    main_menu = NULL;

}
