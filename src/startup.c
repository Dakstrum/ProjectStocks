
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_video.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "rendering.h"
#include "log.h"

static DrawObject *video_object   = NULL;
static DrawObject *loading_object = NULL;
bool is_done_loading = false;

void InitializeStartUpSequence() 
{

    CreateNewDrawLayer();
    video_object = GetDrawObjectFromJsonLayer("StartUpVideo");
    video_object->video.is_repeating = false;
    video_object->video.start_video_immediately = true;
    AddObjectToDrawLayer(video_object);
    
}

void InitializeLoadingSequence() 
{

    CreateNewDrawLayer();
    loading_object = GetDrawObjectFromJsonLayer("LoadingVideo");
    loading_object->video.is_repeating = true;
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

    if (loading_object == NULL)
        InitializeLoadingSequence();

    DrawLayers();
    if (!al_is_video_playing(loading_object->video.video)) {

        if(is_done_loading) {

            ClearDrawLayers();
            SwitchToRenderingStocksMenu();

        } else {

            ClearDrawLayers();
            InitializeLoadingSequence();

        }
        
    }  

}