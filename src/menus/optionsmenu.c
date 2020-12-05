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
#include "pausemenus.h"
#include "mainmenu.h"

static MenuWithChilds *options_menu = NULL;
static MenuWithChilds *display_menu = NULL;
static MenuWithChilds *audio_menu   = NULL;

static DrawObject *fullscreen_button = NULL;

static DrawObject *resolution_textobject = NULL;
static DrawObject *master_textobject     = NULL;
static DrawObject *music_textobject      = NULL;
static DrawObject *fx_textobject         = NULL;

static DrawObject *display_scrollbox = NULL;

char* OptionsMenu_GetFormatedResolutionForText();
int OptionsMenu_DisplayMenu_RemoveResolutionScrollbox();
void OptionsMenu_AudioMenu_Toggle();
void OptionsMenu_AudioMenu_InitText();

typedef struct Resolution_OptionItem 
{

    char *resolution;
    int width;
    int height;

} Resolution_OptionItem;

#define NUM_RESOLUTIONS 6
static Resolution_OptionItem resolutions[NUM_RESOLUTIONS] = 
{
    {"1920x1080", 1920, 1080},
    {"1366x768",  1366, 768},
    {"1440x900",  1440, 900},
    {"1536x864",  1536, 864},
    {"1024x768",  1024, 768},
    {"1280x720",  1280, 720}
};

void OptionsMenu_DisplayMenu_ResolutionClick(char *scroll_box_content, unsigned short int index);
void OptionsMenu_DisplayMenu_InitResolutionScrollbox();
void OptionsMenu_DisplayMenu_UpdateFullScreenButton();

void OptionsMenu_Toggle()
{

    if (options_menu == NULL) {

        CreateNewDrawLayer();
        options_menu = GetJSONMenuAndAddToDrawLayer("OptionsMenu");
        
    } else {

        ClearCurrentDrawLayer();
        PauseMenu_Init();
        options_menu = NULL;
        
    }

}

void OptionsMenu_MainMenuVersonToggle()
{

    if (options_menu == NULL) {

        CreateNewDrawLayer();
        options_menu = GetJSONMenuAndAddToDrawLayer("OptionsMenu");
        
    } else {

        ClearCurrentDrawLayer();
        options_menu = NULL;

    }
}

void OptionsMenu_DisplayMenu_Toggle()
{

    if(options_menu)
        OptionsMenu_MainMenuVersonToggle();

    else if(audio_menu)
        OptionsMenu_AudioMenu_Toggle();

    if (display_menu == NULL) {

        CreateNewDrawLayer();
        display_menu = GetJSONMenuAndAddToDrawLayer("DisplayMenu");

        resolution_textobject = GetJSONObjectAndAddToDrawLayer("DisplayMenuresolutionTextObject");
        SetTextContent(resolution_textobject, "%s", OptionsMenu_GetFormatedResolutionForText());
            
    } else {

        display_scrollbox = NULL;
        ClearCurrentDrawLayer();
        display_menu = NULL;

    }

    fullscreen_button = GetDrawObjectByName("DisplayMenuFullScreenButtonObject");
    OptionsMenu_DisplayMenu_UpdateFullScreenButton();

}

void OptionsMenu_AudioMenu_Toggle()
{

    if(options_menu)
        OptionsMenu_MainMenuVersonToggle();

    else if(display_menu)
        OptionsMenu_DisplayMenu_Toggle();

    if (audio_menu == NULL) {

        CreateNewDrawLayer();
        audio_menu = GetJSONMenuAndAddToDrawLayer("AudioMenu");
        OptionsMenu_AudioMenu_InitText();

            
    } else {

        ClearCurrentDrawLayer();
        audio_menu = NULL;

    }

}

void OptionsMenu_AudioMenu_InitText()
{

    master_textobject = GetJSONObjectAndAddToDrawLayer("AudioMenumaTextObject");
    music_textobject  = GetJSONObjectAndAddToDrawLayer("AudioMenumuTextObject");
    fx_textobject     = GetJSONObjectAndAddToDrawLayer("AudioMenufxTextObject"); 

    SetTextContent(master_textobject, "%d", GetAudioMasterSetting());
    SetTextContent(music_textobject,  "%d", GetAudioMusicSetting());
    SetTextContent(fx_textobject,     "%d", GetAudioFxSetting());

}

int OptionsMenu_DisplayMenu_RemoveResolutionScrollbox()
{

    if(display_scrollbox)
    {

        RemoveDrawObject(display_scrollbox);
        display_scrollbox = NULL;
        return 1;

    }
    return 0;

}

char* OptionsMenu_GetFormatedResolutionForText()
{

    for(int i = 0; i < NUM_RESOLUTIONS; i++)
        if(resolutions[i].width == Window_Width() && resolutions[i].height == Window_Height())
            return resolutions[i].resolution;

    return NULL;
}

void OptionsMenu_DisplayMenu_ResolutionClick(char *scroll_box_content, unsigned short int index)
{

    for (size_t i = 0; i < NUM_RESOLUTIONS;i++) {

        if (strcmp(scroll_box_content, resolutions[i].resolution) == 0) {

            Window_Resize(resolutions[i].width, resolutions[i].height);    
            break;

        }

    }

    SetTextContent(resolution_textobject, "%s", OptionsMenu_GetFormatedResolutionForText());
    OptionsMenu_DisplayMenu_RemoveResolutionScrollbox();
    OptionsMenu_DisplayMenu_UpdateFullScreenButton();

}

void OptionsMenu_DisplayMenu_InitResolutionScrollbox()
{
    if(OptionsMenu_DisplayMenu_RemoveResolutionScrollbox())
        return;
        
    display_scrollbox = Scrollbox_Create();

    display_scrollbox->x      = 1173;
    display_scrollbox->y      = 540;
    display_scrollbox->width  = 288;
    display_scrollbox->height = 510;
    display_scrollbox->scrollbox.vertical_spacing = 69;
    display_scrollbox->asset_path = "assets/images/generalpurposemenus/optionsmenu/selectoptionresolutionbox.png";

    display_scrollbox->scrollbox.num_items = NUM_RESOLUTIONS;
    display_scrollbox->scrollbox.box_click = &OptionsMenu_DisplayMenu_ResolutionClick;

    InitScrollboxVectors(display_scrollbox);

    for (size_t i = 0; i < NUM_RESOLUTIONS;i++) {

        ScrollboxText text = {30, 5, NULL, 40, resolutions[i].resolution};
        Vector_PushBack(display_scrollbox->scrollbox.text_content[i], &text);

    }

    AddObjectToDrawLayer(display_scrollbox);

}

void OptionsMenu_DisplayMenu_UpdateFullScreenButton()
{

    WindowSettings window_settings = GetWindowSettings();
    
    if(window_settings.fullscreen)
        SetButtonTint(fullscreen_button, GetRGBA(0, 255, 0, 255));
        
    else
        SetButtonTint(fullscreen_button, GetRGBA(255, 0, 0, 255));

}

void OptionsMenu_Exit_CB()
{
    if(MainMenu_Active())
        OptionsMenu_MainMenuVersonToggle();
    else
        OptionsMenu_Toggle();

}

void OptionsMenu_DisplayMenu_Exit_CB()
{

    if(MainMenu_Active()) {

        OptionsMenu_DisplayMenu_Toggle();

    } else {

        OptionsMenu_DisplayMenu_Toggle();
        PauseMenu_Init();

    }

}

void OptionsMenu_AudioMenu_Exit_CB()
{

    if(MainMenu_Active()) {

        OptionsMenu_AudioMenu_Toggle();

    } else {

        OptionsMenu_AudioMenu_Toggle();
        PauseMenu_Init();

    }

}

void OptionsMenu_DisplayMenu_FullScreen_CB()
{

    Window_FullScreen();
    OptionsMenu_DisplayMenu_UpdateFullScreenButton();
    SetTextContent(resolution_textobject, "%s", OptionsMenu_GetFormatedResolutionForText());

}

void OptionsMenu_DisplayMenu_Resolution_CB()
{

    OptionsMenu_MainMenuVersonToggle();
    OptionsMenu_DisplayMenu_Toggle();

}

void OptionsMenu_AudioMenu_MasterDecrease()
{



}

void OptionsMenu_AudioMenu_MasterIncrease()
{



}

void OptionsMenu_AudioMenu_MusicDecrease()
{



}

void OptionsMenu_AudioMenu_MusicIncrease()
{



}

void OptionsMenu_AudioMenu_FxDecrease()
{



}

void OptionsMenu_AudioMenu_FxIncrease()
{



}

void OptionsMenu_Clean()
{

    options_menu = NULL;
    display_menu = NULL;
    audio_menu   = NULL;
    fullscreen_button = NULL;
    resolution_textobject = NULL;
    master_textobject     = NULL;
    music_textobject      = NULL;
    fx_textobject         = NULL;
    display_scrollbox = NULL;

}