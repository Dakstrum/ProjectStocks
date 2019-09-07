
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
    AddVideoToDrawLayer(video_object, true);
    
}

void StartUpSequence() 
{

    if (video_object == NULL)
        InitializeStartUpSequence();

    DrawLayers();
    if (!al_is_video_playing(video_object->member.video.video)) {
        ClearDrawLayers();
        SwitchToRenderingMainMenu();
    }
        
}