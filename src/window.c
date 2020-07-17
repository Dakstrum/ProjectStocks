#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_video.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "log.h"
#include "drawlayers.h"
#include "rendering.h"
#include "dbaccess.h"

typedef struct DisplayFlagMap {

    ScreenSetting display_setting;
    int allegro_flag;

} DisplayFlagMap;

static ALLEGRO_DISPLAY *display = NULL;
static WindowSettings window_settings;

static DisplayFlagMap flag_maps[3] = 
{

    {FULLSCREEN, ALLEGRO_FULLSCREEN},
    {WINDOWED, ALLEGRO_WINDOWED},
    {BORDERLESS, ALLEGRO_FRAMELESS}

};

void InitializeDisplay() 
{
    
    window_settings = GetWindowSettings();

    al_set_new_display_flags(flag_maps[window_settings.screen_flag].allegro_flag);
    
    display = al_create_display(window_settings.width, window_settings.height);
    InitializeDrawLayers(display);

}

void HandleWindowEvents(ALLEGRO_EVENT event) 
{

    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        SetCleanUpToTrue();

}

void CleanUpDisplay() 
{

    al_destroy_display(display);

}

void ChangeDisplay(int width, int height) 
{

    al_resize_display(display, width, height);

}

ALLEGRO_DISPLAY *GetDisplay() 
{

    return display;

}

void SetDisplayIcon()
{

    al_set_display_icon(display, al_load_bitmap("assets/icon.tga"));

}