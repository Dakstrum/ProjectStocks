#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

static MenuWithChilds *main_menu = NULL;
static DrawObject *verson_object = NULL;

void DynamicText();
void MainMenuRenderLogic();
void CleanMenus();

void InitializeMainMenu() 
{

    if (CreateNewDrawLayer() == -1) {

        Log("ERROR: MainMenu could not create new draw layer");
        return;

    }

    SetInGameStatus(0);
    
    main_menu = GetJSONMenuAndAddToDrawLayer("MainMenu");
    
    DynamicText();
    MainMenuRenderLogic();
    StopSimulation();
    CleanMenus();

}

void MainMenuRenderLogic()
{

    // TEST CODE
    if (verson_object == NULL)
        return;

    //SetTextContent(verson_object, "Version is %d", 0);

}

void DynamicText() 
{

    // TEST CODE
    //verson_object = GetDrawObjectFromJsonLayer("MainMenuWebsiteText");
    //AddObjectToDrawLayer(verson_object);

}

void ButtonTintTest() 
{

    // TEST CODE
    DrawObject *object = GetDrawObjectByName("StartButton");
    if (object == NULL)
        return;

    SetButtonTint(object, GetRGBA(217, 17, 39, 255));
}

void ScrollBoxClick(char *scroll_box_content, unsigned short int index)
{

    LogF("Got click %s", scroll_box_content);
    
}

void DisplayTempScrollBox() 
{

    // TEST CODE
    DrawObject *object = CreateScrollBoxObject();
    object->x          = 100;
    object->y          = 100;
    object->width      = 300;
    object->height     = 500;
    object->asset_path = "assets/images/companyicons/StocksBox.png";

    object->scrollbox.num_items        = 2;
    object->scrollbox.box_click        = &ScrollBoxClick;
    object->scrollbox.text_content     = malloc(sizeof(char *) * 2);
    object->scrollbox.text_content[0]  = GetFormattedPointer("WeBeHard");
    object->scrollbox.text_content[1]  = GetFormattedPointer("Unimpressive Games");

    AddObjectToDrawLayer(object);

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