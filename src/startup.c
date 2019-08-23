
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_video.h>

#include "drawlayers.h"
#include "rendering.h"
#include "log.h"

static ALLEGRO_VIDEO *startup_video;
static bool played_video        = false;
static int video_draw_layer     = -1;
static char *startup_video_path = "assets/videos/startup/startup.ogv";

void CleanUpStartUpSequence();

void InitializeStartUpSequence() 
{

    video_draw_layer         = CreateNewDrawLayer();
    DrawObject *video_object = CreateNewDrawObject();
    video_object->type       = VIDEO;
    video_object->scale_to_entire_screen = true;
    video_object->should_this_be_drawn   = true;
    video_object->x                      = 0;
    video_object->y                      = 0;
    strncpy(video_object->member.video.video_path, startup_video_path , strlen(startup_video_path ));
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