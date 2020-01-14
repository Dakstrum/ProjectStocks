
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_video.h>

#include "simulation.h"
#include "jsonlayer.h"
#include "drawlayers.h"
#include "rendering.h"
#include "log.h"

static DrawObject *video_object = NULL;
DrawObject *loading_object      = NULL;

void InitializeStartUpSequence() 
{

    CreateNewDrawLayer();
    video_object             = GetDrawObjectFromJsonLayer("StartUpVideo");
    video_object->bit_flags |= VIDEO_SHOULD_START_IMMEDIATELY;
    AddObjectToDrawLayer(video_object);
    
}

void InitializeLoadingSequence() 
{

    CreateNewDrawLayer();
    loading_object           = GetDrawObjectFromJsonLayer("LoadingVideo");
    video_object->bit_flags |= VIDEO_SHOULD_REPEAT | VIDEO_SHOULD_START_IMMEDIATELY;
    AddObjectToDrawLayer(loading_object);
    
}

void StartUpSequence() 
{

    if (video_object == NULL)
        InitializeStartUpSequence();

    if (!al_is_video_playing(video_object->video.video)) {
        
        ClearDrawLayers();
        SwitchToRenderingMainMenu();

    }
        
}

void LoadingSequence() 
{

    if (loading_object == NULL) {   

        ClearDrawLayers();
        InitializeLoadingSequence();
        
    }

    if (IsSimulationDone()) {

        ClearDrawLayers();
        SwitchToRenderingStocksMenu();

    }

}