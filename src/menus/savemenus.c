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
#include "dbaccess.h"

static MenuWithChilds *load_save_menu    = NULL;
static MenuWithChilds *new_save_menu     = NULL;

void LoadSaveScrollBoxClick(char *scroll_box_content)
{
    
    LogF("save name: %s", scroll_box_content);
    
}

void AddSaveContentToScrollBox(DrawObject *object)
{
    GetAmountOfSaves();
    for(int i; i < GetAmountOfSaves(); i++)
        object->scrollbox.text_content[i]  = GetSaveNameFromSaveId(i+1);
}

void DisplayLoadSaveScrollBox() 
{

    DrawObject *object = CreateScrollBoxObject();

    object->type = SCROLLBOX;
    object->x          = 535;
    object->y          = 226;
    object->width      = 288;
    object->height     = 603;
    object->asset_path = "assets/images/companyicons/SaveBox.png";

    object->scrollbox.num_items        = GetAmountOfSaves();
    object->scrollbox.box_click        = &LoadSaveScrollBoxClick;
    object->scrollbox.text_content     = malloc(sizeof(char *) * 2);

    AddSaveContentToScrollBox(object);
    AddObjectToDrawLayer(object);

}

void InitializeLoadSaveMenu() 
{

    
    if (CreateNewDrawLayer() == -1) {

        Log("ERROR: LoadMenu could not create new draw layer");
        return;

    }

    load_save_menu = GetMenuWithChildsFromJsonLayer("LoadSaveMenu");
    AddMenuWithChildsToDrawLayer(load_save_menu);
    DisplayLoadSaveScrollBox();

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

void LoadSaveMenuLoadButtonCallBack()
{

    SwitchToLoadingScreen();

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
