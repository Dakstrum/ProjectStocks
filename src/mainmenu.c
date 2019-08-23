
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>


#include "drawlayers.h"
#include "log.h"

static char *main_menu_asset_path = "assets/images/mainmenu/mainmenu.png";

void InitializeMainMenu() 
{

    DrawObject *main_menu_object = CreateNewDrawObject();
    if (CreateNewDrawLayer() == -1) {

        Log("STUB: MainMenu could not create new draw layer");
        return;

    }
    main_menu_object->type                   = MENU;
    main_menu_object->should_this_be_drawn   = true;
    main_menu_object->x                      = 0;
    main_menu_object->y                      = 0;
    main_menu_object->scale_to_entire_screen = true;
    strncpy(main_menu_object->member.menu.picture_path, main_menu_asset_path, strlen(main_menu_asset_path));

    AddMenuToDrawLayer(main_menu_object);
    Log("Created Main Menu Draw Layer");

}


void RenderMainMenu() 
{

    DrawLayers();

}

void CleanUpMainMenu() 
{

}