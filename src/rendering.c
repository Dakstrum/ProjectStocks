#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_video.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "log.h"
#include "shared.h"
#include "mainmenu.h"
#include "drawlayers.h"
#include "rendering.h"
#include "startup.h"
#include "dbaccess.h"
#include "stocksmenu.h"
#include "newsmenu.h"
#include "accountmenu.h"
#include "cardsmenu.h"
#include "window.h"
#include "draw.h"

void InitializeAddons();

void CleanUpRendering();
void CleanUpDisplay();
void CleanUpAddons();

static void (*RenderLogic)() = &StartUpSequence;

void InitializeRendering() 
{

    InitializeAddons();

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

void HandleRendering(double dt) 
{

    if (RenderLogic != NULL)
        RenderLogic();

    DrawLayers();
    DrawFrameTiming(dt);
    al_flip_display();

}

void CleanUpRendering() 
{

    CleanUpDisplay();
    CleanUpAddons();

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

    MainMenu_Init();
    RenderLogic = &MainMenu_RenderLogic;

}

void SwitchToLoadingScreen()
{

    RenderLogic = &LoadingSequence;

}

void SwitchToRenderingStocksMenu()
{

    InitializeStocksMenu();
    RenderLogic = &StocksMenusRenderLogic;

}

void SwitchToRenderingNewsMenu()
{

    InitializeNewsMenu();
    RenderLogic = &NewsMenuRenderLogic;

}

void SwitchToRenderingAccountMenu()
{

    AccountMenu_Init();
    RenderLogic = &AccountMenu_RenderLogic;

}

void SwitchToRenderingCardsMenu()
{

    CardsMenu_Init();
    RenderLogic = &CardsMenu_RenderLogic;

}