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
#include "stocksmenu.h"

void InitializeDisplay();
void InitializeAddons();

void CleanUpRendering();
void CleanUpDisplay();
void CleanUpAddons();

static void (*Render)()         = &StartUpSequence;
static ALLEGRO_DISPLAY *display = NULL;

void InitializeRendering() 
{

    InitializeDisplay();
    InitializeAddons();

}

void InitializeDisplay() 
{

    display = al_create_display(1920, 1080);
    InitializeDrawLayers(display);
    
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

    Render();
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
    al_shutdown_ttf_addon();
    al_shutdown_primitives_addon();

}

void SwitchToRenderingMainMenu() 
{

    InitializeMainMenu();
    Render = &RenderMainMenu;
    Log("MainMenu Initialized");

}

void SwitchToLoadingScreen()
{

    Render = &LoadingSequence;
    Log("Loading Initialized");

}

void SwitchToRenderingStocksMenu()
{

    InitializeStocksMenu();
    Render = &RenderStocksMenu;
    Log("Stocks Menu Initialized");

}

ALLEGRO_DISPLAY *GetDisplay() 
{

    return display;

}