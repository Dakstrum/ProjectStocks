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

#include "stocksmenu.h"
#include "generalpurposemenus.h"
#include "savemenus.h"

#include "linkopener.h"
#include "rendering.h"
#include "dbaccess.h"

static MenuWithChilds *main_menu = NULL;
static DrawObject *VersionObject = NULL;

void DynamicText();
void MainMenuRenderLogic();


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

void DynamicText() 
{

    // TEST CODE
    VersionObject = GetDrawObjectFromJsonLayer("MainMenuWebsiteText");
    AddObjectToDrawLayer(VersionObject);

}

void MainMenuRenderLogic()
{

    // TEST CODE
    if (VersionObject == NULL)
        return;

    static int i = 0;
    SetTextContent(VersionObject, "Version is %d", i);

}

void ButtonTintTest() 
{

    // TEST CODE
    DrawObject *object = GetDrawObjectByName("StartButton");
    if (object == NULL)
        return;

    SetButtonTint(object, GetRGBA(217, 17, 39, 255));
}

void InitializeMainMenu() 
{

    
    if (CreateNewDrawLayer() == -1) {

        Log("ERROR: MainMenu could not create new draw layer");
        return;

    }
    
    main_menu = GetMenuWithChildsFromJsonLayer("MainMenu");
    AddMenuWithChildsToDrawLayer(main_menu);
    
    DynamicText();
    MainMenuRenderLogic();
    ButtonTintTest();

}

void CleanUpMainMenu() 
{

    if (main_menu != NULL)
        free(main_menu);
    
    main_menu = NULL;

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