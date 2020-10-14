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

typedef struct Resolution 
{

    char *resolution;
    int width;
    int height;

} Resolution;

#define NUM_RESOLUTIONS 6
static Resolution resolutions[NUM_RESOLUTIONS] = 
{
    {"1920x1080", 1920, 1080},
    {"1366x768", 1366, 768},
    {"1440x900", 1440, 900},
    {"1536x864", 1536, 864},
    {"1024x768", 1024, 768},
    {"1280x720", 1280, 720}
};

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

    for (size_t i = 0; i < NUM_RESOLUTIONS;i++) {

        if (strcmp(scroll_box_content, resolutions[i].resolution) == 0) {

            Window_Resize(resolutions[i].width, resolutions[i].height);
            break;

        }

    }

    UpdateFullScreenButton();

}

void InitalizeResolutionScrollbox()
{

    DrawObject *object = CreateScrollBoxObject();

    object->x      = 1060;
    object->y      = 410;
    object->width  = 288;
    object->height = 310;
    object->scrollbox.vertical_spacing = 69;
    object->asset_path = "assets/images/generalpurposemenus/optionsmenu/selectoptionresolutionbox.png";

    object->scrollbox.num_items = NUM_RESOLUTIONS;
    object->scrollbox.box_click = &ChangeResolutionClick;

    InitScrollboxVectors(object);

    for (size_t i = 0; i < NUM_RESOLUTIONS;i++) {

        ScrollboxText text = {30, 5, NULL, 40, resolutions[i].resolution};
        Vector_PushBack(object->scrollbox.text_content[i], &text);

    }

    AddObjectToDrawLayer(object);

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