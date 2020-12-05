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

void GeneralPurposeMenus_CleanAllMenus();

void Test_Animations() 
{

    DrawObject *object = CreateNewDrawObject();
    object->type       = MENU;
    object->asset_path = "assets/images/all_buttons/button2.png";
    object->x          = 100;
    object->y          = 0;
    AddObjectToDrawLayer(object);
    unsigned int id = Animate_MoveDrawObject(object, 0, 0, 100);
    assert(Animate_FinishedMoveAnimation(id) == false);

}

void MainMenuRenderLogic()
{

}

void InitializeMainMenu() 
{

    if (CreateNewDrawLayer() == -1) {

        Log("ERROR: MainMenu could not create new draw layer");
        return;

    }

    Account_SetInGameStatus(0);
    
    main_menu = GetJSONMenuAndAddToDrawLayer("MainMenu");
    
    MainMenuRenderLogic();
    GeneralPurposeMenus_CleanAllMenus();

}


int IsInMainMenu()
{
    if(main_menu)
        return 1;
    else
        return 0;

}

void Start_BCB()
{
    
    ClearDrawLayers();
    InitializeLoadSaveMenu();
    main_menu = NULL;
    
}


void MainMenuOptions_BCB()
{
    
    ToggleMainMenuOptionsMenu();
    

}

void MainMenuExit_BCB()
{

    SetCleanUpToTrue();

}

void GitHub_BCB()
{

    OpenLink("https://www.github.com");

}

void Twitter_BCB()
{

    OpenLink("https://twitter.com/Dakstrum");

}

void Youtube_BCB()
{

    OpenLink("https://www.youtube.com/channel/UCIW4bSzn44v08ttyRMT5z2w");

}

void Website_BCB()
{

    OpenLink("https://www.dakstrum.com");

}


void CleanUpMainMenu() 
{

    if (main_menu != NULL)
        free(main_menu);
    
    main_menu = NULL;

}
