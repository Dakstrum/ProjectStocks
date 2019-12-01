#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"
#include "shared.h"
#include "stocksmenu.h"
#include "generalpurposemenus.h"
#include "linkopener.h"
#include "rendering.h"

static MenuWithChilds *main_menu = NULL;

void DisplayTempScrollBox() 
{

    DrawObject *object = CreateNewDrawObject();

    object->type = SCROLLBOX;
    object->should_this_be_drawn = true;
    object->x      = 100;
    object->y      = 100;
    object->width  = 100;
    object->height = 500;

    object->scrollbox.vertical_spacing = 20;
    object->scrollbox.vertical_offset  = 0;
    object->scrollbox.num_items        = 2;
    object->scrollbox.text_content     = malloc(sizeof(char *) * 2);
    object->scrollbox.text_content[0]  = "WeBeHard";
    object->scrollbox.text_content[1]  = "Unimpressive Games";
    object->scrollbox.text_style       = malloc(sizeof(TextStyle));
    object->scrollbox.text_style->font_size = 20;
    object->scrollbox.text_style->a = 255;
    object->scrollbox.text_style->r = 255;
    object->scrollbox.text_style->g = 255;
    object->scrollbox.text_style->b = 255;
    object->scrollbox.text_style->font_path = "assets/font/Open_Sans/OpenSans-Regular.ttf";

    AddObjectToDrawLayer(object);

}

void InitializeMainMenu() 
{

    
    if (CreateNewDrawLayer() == -1) {

        Log("ERROR: MainMenu could not create new draw layer");
        return;

    }
    
    main_menu = GetMenuWithChildsFromJsonLayer("MainMenu");
    AddMenuWithChildsToDrawLayer(main_menu);
    DisplayTempScrollBox();

}

void CleanUpMainMenu() 
{

    if (main_menu != NULL)
        free(main_menu);
    
    main_menu = NULL;

}

//Menu Button Callbacks
void MainMenuStartButtonCallBack()
{
    
    SwitchToLoadingScreen();

}

void MainMenuOptionsButtonCallBack()
{
    
    ToggleOptionsMenu();

}

void MainMenuExitButtonCallBack()
{

    SetCleanUpToTrue();

}

void MainMenuGitHubButtonCallBack()
{

    OpenLink("www.github.com");

}

void MainMenuTwitterButtonCallBack()
{

    OpenLink("https://twitter.com/DSectorStudios");

}

void MainMenuYoutubeButtonCallBack()
{

    OpenLink("https://www.youtube.com/channel/UCIW4bSzn44v08ttyRMT5z2w");

}

void MainMenuWebsiteButtonCallBack()
{

    OpenLink("https://www.darksectorstudios.com");

}