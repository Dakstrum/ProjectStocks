
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
    video_object = GetDrawObjectFromJsonLayer("StartUpVideo");
    video_object->video.should_repeat           = false;
    video_object->video.start_video_immediately = true;
    AddObjectToDrawLayer(video_object);
    
}

void InitializeLoadingSequence() 
{

    CreateNewDrawLayer();
    loading_object = GetDrawObjectFromJsonLayer("LoadingVideo");
    loading_object->video.should_repeat           = true;
    loading_object->video.start_video_immediately = true;
    AddObjectToDrawLayer(loading_object);
    
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

void LoadingSequence() 
{

    if (loading_object == NULL) {   

        ClearDrawLayers();
        InitializeLoadingSequence();
    }

    DrawLayers();
    if (IsSimulationDone()) {

        ClearDrawLayers();
        SwitchToRenderingStocksMenu();

    }

}