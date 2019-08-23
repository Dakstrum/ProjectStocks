
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_video.h>

#include "drawlayers.h"
#include "rendering.h"
#include "log.h"

static char *startup_video_path = "assets/videos/startup/startup.ogv";
static DrawObject *video_object = NULL;

void InitializeStartUpSequence() 
{

    CreateNewDrawLayer();
    video_object                         = CreateNewDrawObject();
    video_object->type                   = VIDEO;
    video_object->scale_to_entire_screen = true;
    video_object->should_this_be_drawn   = true;
    video_object->x                      = 0;
    video_object->y                      = 0;
    strncpy(video_object->member.video.video_path, startup_video_path , strlen(startup_video_path));
    AddVideoToDrawLayer(video_object, true);
    
}

void StartUpSequence() 
{

    if (video_object == NULL)
        InitializeStartUpSequence();

    DrawLayers();
    if (!al_is_video_playing(video_object->member.video.video)) {

        SwitchToRenderingMainMenu();

    }

}