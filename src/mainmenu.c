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

//static DrawObject *main_menu     = NULL;
//static DrawObject *version_text  = NULL;
static MenuWithChilds *main_menu    = NULL;


void InitializeMainMenu() 
{

    
    if (CreateNewDrawLayer() == -1) {

        Log("ERROR: MainMenu could not create new draw layer");
        return;

    }
    
    main_menu = GetMenuWithChildsFromJsonLayer("MainMenu");
    AddMenuWithChildsToDrawLayer(main_menu);

}

void RenderMainMenu() 
{
    DrawLayers();
}

void CleanUpMainMenu() 
{
    free(main_menu);
}

void InitStartGame()
{
    InitializeStocksMenu();
}

//Menu Button Callbacks
void MainMenuStartButtonCallBack()
{

    InitStartGame();

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