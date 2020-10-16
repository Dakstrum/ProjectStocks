#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"

#include "text.h"
#include "drawobject.h"
#include "button.h"
#include "scrollbox.h"
#include "drawlayers.h"

#include "log.h"
#include "shared.h"

#include "accountmenu.h"
#include "cardsmenu.h"
#include "generalpurposemenus.h"
#include "newsmenu.h"
#include "optionsmenu.h"
#include "pausemenus.h"
#include "savemenus.h"
#include "stocksmenu.h"

#include "simulation.h"
#include "linkopener.h"
#include "rendering.h"
#include "dbaccess.h"
#include "account.h"
#include "drawlayerutils.h"

#include "animations.h"

static MenuWithChilds *main_menu = NULL;

void CleanMenus();

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

    SetInGameStatus(0);
    
    main_menu = GetJSONMenuAndAddToDrawLayer("MainMenu");
    
    //Test_Animations();
    MainMenuRenderLogic();
    StopSimulation();
    CleanMenus();

}

void Start_BCB()
{
    
    ClearDrawLayers();
    InitializeLoadSaveMenu();
    
}

void MainMenuOptions_BCB()
{
    
    ToggleOptionsMenu();

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

void CleanMenus() 
{

    CleanAccountMenu();
    CleanSaveMenu();
    CleanCardsMenu();
    CleanGeneralPurposeMenu();
    CleanOptionsMenu();
    CleanPauseMenu();
    CleanStocksMenu();

}