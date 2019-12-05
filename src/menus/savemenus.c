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
#include "account.h"

static MenuWithChilds *load_save_menu    = NULL;
static MenuWithChilds *new_save_menu     = NULL;

void InitializeLoadSaveMenu() 
{

    
    if (CreateNewDrawLayer() == -1) {

        Log("ERROR: LoadMenu could not create new draw layer");
        return;

    }
    
    load_save_menu = GetMenuWithChildsFromJsonLayer("LoadSaveMenu");
    AddMenuWithChildsToDrawLayer(load_save_menu);

}

void InitializeNewSaveMenu() 
{

    
    if (CreateNewDrawLayer() == -1) {

        Log("ERROR: NewMenu could not create new draw layer");
        return;

    }
    
    new_save_menu = GetMenuWithChildsFromJsonLayer("NewSaveMenu");
    AddMenuWithChildsToDrawLayer(new_save_menu);

}

void CleanUpLoadSaveMenu() 
{

    if (load_save_menu != NULL)
        free(load_save_menu);
    
    load_save_menu = NULL;

}

void CleanUpNewSaveMenu() 
{

    if (new_save_menu != NULL)
        free(new_save_menu);
    
    new_save_menu = NULL;

}


//Load SaveButton Callbacks

void NewSaveButtonCallBack()
{
    CreateNewDrawLayer();
    new_save_menu = GetMenuWithChildsFromJsonLayer("NewSaveMenu");
    AddMenuWithChildsToDrawLayer(new_save_menu);

}

//New Save Button Callbacks
void NewSaveMenuBackButtonCallBack()
{

    ClearCurrentDrawLayer();
    
}

void NewSaveMenuCreateButtonCallBack()
{
    
    char *save_name_in_text_box   = GetTextFromTextBox("SaveNameTextBox");
    char *player_name_in_text_box = GetTextFromTextBox("PlayerNameTextBox");

    CreateNewSave(save_name_in_text_box, player_name_in_text_box);

}
