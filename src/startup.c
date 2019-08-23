
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_video.h>

#include "drawlayers.h"
#include "rendering.h"
#include "log.h"

static ALLEGRO_VIDEO *startup_video;
static bool played_video    = false;
static int video_draw_layer = -1;

void CleanUpStartUpSequence();

void InitializeStartUpSequence() 
{

    //video_draw_layer = CreateNewDrawLayer();
    //AddVideoToDrawLayer();
    //startup_video = al_open_video("assets/videos/startup/startup.ogv");
    
}

void StartUpSequence() 
{

    /*
    Log("STUB: StartUpSequence");

    if (!played_video) {

        played_video = true;
        al_start_video(startup_video);

    } else if (played_video && !al_is_video_playing(startup_video)) {

        SwitchToRenderingMainMenu();
        CleanUpStartUpSequence();

    }
    */

    SwitchToRenderingMainMenu();
    CleanUpStartUpSequence();

}

void CleanUpStartUpSequence() 
{

    played_video = false;
    al_close_video(startup_video);

}