
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"

static DrawObject *main_menu    = NULL;
static DrawObject *version_text = NULL;

void InitializeMainMenu() 
{

    
    if (CreateNewDrawLayer() == -1) {

        Log("STUB: MainMenu could not create new draw layer");
        return;

    }
    main_menu    = GetDrawObjectFromJsonLayer("MainMenu");
    version_text = GetDrawObjectFromJsonLayer("Version");

    LogF("Version_text = %s with x = %f, y = %f", version_text->name, version_text->x, version_text->y);

    AddObjectToDrawLayer(main_menu);
    AddObjectToDrawLayer(version_text);
    Log("Created Main Menu Draw Layer");

}


void RenderMainMenu() 
{

    DrawLayers();

}

void CleanUpMainMenu() 
{

}