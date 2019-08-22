#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_video.h>

#include "log.h"
#include "shared.h"
#include "mainmenu.h"
#include "drawlayers.h"
#include "rendering.h"
#include "startup.h"

void InitializeRendering();
void InitializeDisplay();
void InitializeAddons();

void RenderingLoop();
void RenderFrame();

void CleanUpRendering();
void CleanUpDisplay();
void CleanUpAddons();

void HandleWindowEvents();

static float FPS                         = 60.0; 
static ALLEGRO_DISPLAY *display          = NULL;
static ALLEGRO_TIMER *timer              = NULL;
static ALLEGRO_EVENT_QUEUE *event_queue  = NULL;

static void (*Render)()                  = &StartUpSequence;


void *RenderingEntry(ALLEGRO_THREAD *thread, void *arg) 
{

    InitializeRendering();
    RenderingLoop();
    CleanUpRendering();

    Log("Quiting Rendering Thread");
    
    return NULL;

}

void InitializeRendering() 
{

    InitializeDisplay();
    InitializeAddons();

}

void InitializeDisplay() 
{

    display     = al_create_display(1920, 1080);
    timer       = al_create_timer(1.0/FPS);
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    InitializeDrawLayers(display);

}

void InitializeAddons() 
{

    al_init_image_addon();
    al_init_video_addon();

}

void RenderingLoop() 
{

    while (!ShouldICleanUpDisplay()) {

        Render();
        al_flip_display();
        HandleWindowEvents();

    }

}

void HandleWindowEvents() 
{

    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);
    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {

        SetCleanUpToTrue();

    } else if (event.type == ALLEGRO_EVENT_TIMER) {

        // Log();

    }

}

void CleanUpRendering() 
{

    CleanUpDisplay();
    CleanUpAddons();

}

void CleanUpDisplay() 
{

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

}

void CleanUpAddons() 
{

    al_shutdown_image_addon();
    al_shutdown_video_addon();

}

void SwitchToRenderingMainMenu() 
{

    InitializeMainMenu();
    Render = &RenderMainMenu;
    Log("MainMenu Initialized");

}