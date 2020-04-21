#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"
#include "cache.h"
#include "graph.h"
#include "startup.h"
#include "dbaccount.h"
#include "account.h"
#include "simulation.h"
#include "scrollbox.h"
#include "text.h"
#include "generalpurposemenus.h"

static MenuWithChilds *options_menu      = NULL;
static int option_id = 0;

void DisplayOptionsScrollBox();
void AddOptionsContentToOptionsScrollBox(DrawObject *object);
void ChangeResolutionOption(char *scroll_box_content);
void DisplayResolutionOptionsOnScrollbox(DrawObject *object);

void InitializeOptionsMenu() 
{

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: options Menu could not create new draw layer");
        return;
    }

    AddMenuWithChildsToDrawLayer(GetMenuWithChildsFromJsonLayer("OptionsMenu"));
    
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

void OptionsMenuRenderLogic()
{



}

void OptionsMenuExit_BCB()
{

    ToggleOptionsMenu();

}

void OptionsMenuResolution_BCB()
{

    option_id = 1;
    DisplayOptionsScrollBox();

}

void DisplayOptionsScrollBox() 
{

    DrawObject *object = CreateScrollBoxObject();

    object->x      = 1000;
    object->y      = 380;
    object->width  = 288;
    object->height = 310;

    AddOptionsContentToOptionsScrollBox(object);
    AddObjectToDrawLayer(object);

}

void AddOptionsContentToOptionsScrollBox(DrawObject *object)
{

    switch(option_id)
    {

        case 1:
            DisplayResolutionOptionsOnScrollbox(object);
    }
        
}

void ChangeResolutionOption(char *scroll_box_content)
{

    Log("Change resolution");

}

void DisplayResolutionOptionsOnScrollbox(DrawObject *object)
{

    object->asset_path = "assets/images/companyicons/optionbox.png";

    object->scrollbox.num_items        = 6;
    object->scrollbox.box_click        = &ChangeResolutionOption;
    object->scrollbox.text_content     = malloc(sizeof(char *) * object->scrollbox.num_items);

    object->scrollbox.text_content[0]  = GetFormattedPointer("1920x1080");
    object->scrollbox.text_content[1]  = GetFormattedPointer("1366x768");
    object->scrollbox.text_content[2]  = GetFormattedPointer("1440x900");
    object->scrollbox.text_content[3]  = GetFormattedPointer("1536x864");
    object->scrollbox.text_content[4]  = GetFormattedPointer("1024x768");
    object->scrollbox.text_content[5]  = GetFormattedPointer("1280x720");

}
