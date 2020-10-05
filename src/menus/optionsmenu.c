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
#include "dbsettings.h"
#include "button.h"

static MenuWithChilds *options_menu = NULL;
static MenuWithChilds *display_menu = NULL;

static DrawObject *fullscreen_button = NULL;

void ChangeResolutionClick(char *scroll_box_content, unsigned short int index);
void InitalizeResolutionScrollbox();
void UpdateFullScreenButton();

void InitializeOptionsMenu() 
{

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: options Menu could not create new draw layer");
        return;
    }

    options_menu = GetJSONMenuAndAddToDrawLayer("OptionsMenu");
    
}

void InitializeDisplayMenu() 
{

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: display Menu could not create new draw layer");
        return;
    }

    display_menu = GetJSONMenuAndAddToDrawLayer("DisplayMenu");
    
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

void ToggleDisplayMenu()
{

    if (display_menu == NULL) {

        CreateNewDrawLayer();
        display_menu = GetJSONMenuAndAddToDrawLayer("DisplayMenu");
        InitalizeResolutionScrollbox();
        
    } else {

        ClearCurrentDrawLayer();
        display_menu = NULL;
    }

    fullscreen_button = GetDrawObjectByName("DisplayMenuFullScreenButtonObject");

    UpdateFullScreenButton();

}

void ChangeResolutionClick(char *scroll_box_content, unsigned short int index)
{

    if(strcmp(scroll_box_content, "1920x1080") == 0) Window_Resize(1920, 1080);
    if(strcmp(scroll_box_content, "1366x768")  == 0) Window_Resize(1366, 768);
    if(strcmp(scroll_box_content, "1440x900")  == 0) Window_Resize(1440, 900);
    if(strcmp(scroll_box_content, "1536x864")  == 0) Window_Resize(1536, 864);
    if(strcmp(scroll_box_content, "1024x768")  == 0) Window_Resize(1024, 768);
    if(strcmp(scroll_box_content, "1280x720")  == 0) Window_Resize(1280, 720);

    UpdateFullScreenButton();

}

void InitalizeResolutionScrollbox()
{

    DrawObject *object = CreateScrollBoxObject();

    object->x      = 1060;
    object->y      = 410;
    object->width  = 288;
    object->height = 310;
    object->asset_path = "assets/images/savemenus/assets/SaveBox.png";

    object->scrollbox.num_items        = 6;
    object->scrollbox.box_click        = &ChangeResolutionClick;
    object->scrollbox.text_content     = malloc(sizeof(char *) * object->scrollbox.num_items);

    AddObjectToDrawLayer(object);

    object->scrollbox.text_content[0]  = GetFormattedPointer("1920x1080");
    object->scrollbox.text_content[1]  = GetFormattedPointer("1366x768");
    object->scrollbox.text_content[2]  = GetFormattedPointer("1440x900");
    object->scrollbox.text_content[3]  = GetFormattedPointer("1536x864");
    object->scrollbox.text_content[4]  = GetFormattedPointer("1024x768");
    object->scrollbox.text_content[5]  = GetFormattedPointer("1280x720");

}

void UpdateFullScreenButton()
{

    WindowSettings window_settings = GetWindowSettings();
    
    if(window_settings.fullscreen)
        SetButtonTint(fullscreen_button, GetRGBA(0, 255, 0, 255));
        
    else
        SetButtonTint(fullscreen_button, GetRGBA(255, 0, 0, 255));

}

void OptionsMenuExit_BCB()
{

    ToggleOptionsMenu();

}

void DisplayMenuExit_BCB()
{

    ToggleDisplayMenu();

}

void DisplayMenuFullScreen_BCB()
{

    Window_FullScreen();
    UpdateFullScreenButton();

}

void OptionsMenuResolution_BCB()
{

    ToggleDisplayMenu();

}

void CleanOptionsMenu()
{

    options_menu = NULL;
    display_menu = NULL;
    fullscreen_button = NULL;

}