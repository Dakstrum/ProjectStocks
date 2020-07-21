#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"
#include "startup.h"
#include "dbaccount.h"
#include "account.h"
#include "scrollbox.h"
#include "text.h"
#include "generalpurposemenus.h"
#include "dbaccess.h"
#include "rendering.h"
#include "window.h"
#include "drawlayerutils.h"

static MenuWithChilds *options_menu = NULL;

void ChangeResolutionClick(char *scroll_box_content, unsigned short int index);
void InitalizeResolutionScrollbox();

void InitializeOptionsMenu() 
{

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: options Menu could not create new draw layer");
        return;
    }

    options_menu = GetJSONMenuAndAddToDrawLayer("OptionsMenu");
    
}

void OptionsMenuRenderLogic()
{



}

void ToggleOptionsMenu()
{

    if (options_menu == NULL) {

        CreateNewDrawLayer();
        options_menu = GetJSONMenuAndAddToDrawLayer("OptionsMenu");
        
    } else {

        ClearCurrentDrawLayer();
        options_menu = NULL;
    }

}

void ChangeResolutionClick(char *scroll_box_content, unsigned short int index)
{

    if(strcmp(scroll_box_content, "1920x1080") == 0) Window_Resize(1920, 1080);
    if(strcmp(scroll_box_content, "1366x768")  == 0) Window_Resize(1366, 768);
    if(strcmp(scroll_box_content, "1440x900")  == 0) Window_Resize(1440, 900);
    if(strcmp(scroll_box_content, "1536x864")  == 0) Window_Resize(1536, 864);
    if(strcmp(scroll_box_content, "1024x768")  == 0) Window_Resize(1024, 768);
    if(strcmp(scroll_box_content, "1280x720")  == 0) Window_Resize(1280, 720);

}

void InitalizeResolutionScrollbox()
{

    DrawObject *object = CreateScrollBoxObject();

    object->x      = 1000;
    object->y      = 380;
    object->width  = 288;
    object->height = 310;

    AddObjectToDrawLayer(object);

    object->asset_path = "assets/images/companyicons/optionbox.png";

    object->scrollbox.num_items        = 6;
    object->scrollbox.box_click        = &ChangeResolutionClick;
    object->scrollbox.text_content     = malloc(sizeof(char *) * object->scrollbox.num_items);

    object->scrollbox.text_content[0]  = GetFormattedPointer("1920x1080");
    object->scrollbox.text_content[1]  = GetFormattedPointer("1366x768");
    object->scrollbox.text_content[2]  = GetFormattedPointer("1440x900");
    object->scrollbox.text_content[3]  = GetFormattedPointer("1536x864");
    object->scrollbox.text_content[4]  = GetFormattedPointer("1024x768");
    object->scrollbox.text_content[5]  = GetFormattedPointer("1280x720");

}

void OptionsMenuExit_BCB()
{

    ToggleOptionsMenu();

}

void OptionsMenuResolution_BCB()
{

    Log("RESOLUTIONS");

}

void CleanOptionsMenu()
{

    options_menu = NULL;

}