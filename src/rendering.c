#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_video.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include<allegro5/allegro_primitives.h>

#include "log.h"
#include "shared.h"
#include "mainmenu.h"
#include "drawlayers.h"
#include "rendering.h"
#include "startup.h"
#include "dbaccess.h"
#include "stocksmenu.h"
#include "newsmenu.h"

typedef struct DisplayFlagMap {

    ScreenSetting display_setting;
    int allegro_flag;

} DisplayFlagMap;

void InitializeDisplay();
void InitializeAddons();

void CleanUpRendering();
void CleanUpDisplay();
void CleanUpAddons();

void SetDisplayIcon();

static void (*RenderLogic)()    = &StartUpSequence;
static ALLEGRO_DISPLAY *display = NULL;
static WindowSettings window_settings;
static DisplayFlagMap flag_maps[3] = 
{

    {FULLSCREEN, ALLEGRO_FULLSCREEN},
    {WINDOWED, ALLEGRO_WINDOWED},
    {BORDERLESS, ALLEGRO_FRAMELESS}

};

void InitializeRendering() 
{

    SetWindowSettings(GetWindowSettingsFromDB());
    window_settings = GetWindowSettings();

    InitializeDisplay();
    InitializeAddons();
    SetDisplayIcon();

}

void InitializeDisplay() 
{
    
    
    al_set_new_display_flags(flag_maps[window_settings.screen_flag].allegro_flag);
    
    display = al_create_display(window_settings.width, window_settings.height);
    InitializeDrawLayers(display);
    //al_add_new_bitmap_flag(ALLEGRO_NO_PRESERVE_TEXTURE);

}

void InitializeAddons() 
{

    if(!al_init_image_addon())
        Log("Unable to init image addon");
    if(!al_init_video_addon())
        Log("Unable to init video addon");
    if (!al_init_font_addon())
        Log("Unable to init font addon");
    if (!al_init_ttf_addon())
        Log("Unable to init ttf addon");
    if (!al_init_primitives_addon())
        Log("Unable to init primitives addon");
}

void HandleRendering() 
{

    if (RenderLogic != NULL)
        RenderLogic();

    DrawLayers();
    al_flip_display();

}

void HandleWindowEvents(ALLEGRO_EVENT event) 
{

    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        SetCleanUpToTrue();

}

void CleanUpRendering() 
{

    CleanUpDisplay();
    CleanUpAddons();

}

void CleanUpDisplay() 
{

    al_destroy_display(display);

}

void CleanUpAddons() 
{

    al_shutdown_image_addon();
    al_shutdown_video_addon();
    al_shutdown_font_addon();
    al_shutdown_primitives_addon();

}

void SwitchToRenderingMainMenu() 
{

    InitializeMainMenu();
    RenderLogic = &MainMenuRenderLogic;

}

void SwitchToLoadingScreen()
{

    RenderLogic = &LoadingSequence;

}

void SwitchToRenderingStocksMenu()
{

    InitializeStocksMenu();
    RenderLogic = &StocksMenuRenderLogic;

}

void SwitchToRenderingNewsMenu()
{

    InitializeNewsMenu();
    RenderLogic = &NewsMenuRenderLogic;

}


ALLEGRO_DISPLAY *GetDisplay() 
{

    return display;

}

void SetDisplayIcon()
{

    al_set_display_icon(display, al_load_bitmap("assets/icon.tga"));

}