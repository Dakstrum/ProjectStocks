
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"

static DrawObject *main_menu = NULL;

void InitializeMainMenu() 
{

    
    if (CreateNewDrawLayer() == -1) {

        Log("STUB: MainMenu could not create new draw layer");
        return;

    }
    main_menu = GetDrawObjectFromJson("MainMenu");
    AddMenuToDrawLayer(main_menu);
    Log("Created Main Menu Draw Layer");

}


void RenderMainMenu() 
{

    DrawLayers();

}

void CleanUpMainMenu() 
{

}