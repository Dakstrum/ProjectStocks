
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>


#include "drawlayers.h"
#include "log.h"

static char *main_menu_asset_path = "assets/images/mainmenu/mainmenu.png";
static DrawObject *main_menu_object;

void InitializeMainMenu() 
{

    main_menu_object = malloc(sizeof(DrawObject));
    if (!CreateNewDrawLayer()) {

        Log("STUB: MainMenu could not create new draw layer");
        return;

    }
    main_menu_object->type                     = MENU;
    main_menu_object->should_this_be_drawn     = true;
    main_menu_object->member.menu.x            = 0;
    main_menu_object->member.menu.y            = 0;
    main_menu_object->scale_to_entire_screen   = true;
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

    free(main_menu_object);

}