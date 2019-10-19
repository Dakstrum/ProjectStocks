
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_video.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "rendering.h"
#include "log.h"

static DrawObject *video_object = NULL;

void InitializeStartUpSequence() 
{

    CreateNewDrawLayer();
    video_object = GetDrawObjectFromJsonLayer("StartUpVideo");
    video_object->video.start_video_immediately = true;
    AddObjectToDrawLayer(video_object);
    
}

void StartUpSequence() 
{

    if (video_object == NULL)
        InitializeStartUpSequence();

    DrawLayers();
    if (!al_is_video_playing(video_object->video.video)) {
        ClearDrawLayers();
        SwitchToRenderingMainMenu();
    }
        
}