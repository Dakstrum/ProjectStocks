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
#include "dbaccount.h"
#include "mainmenu.h"
#include "simulation.h"

static DrawObject *SaveNameTextObject   = NULL;
static DrawObject *PlayerNameTextObject = NULL;

void UpdateSaveStatsText(char *save_name)
{

    char *player_name = GetPlayerNameFromSaveName(save_name);

    SetTextContent(SaveNameTextObject, "%s",   save_name);
    SetTextContent(PlayerNameTextObject, "%s", player_name);

    free(player_name);

}

void AddSaveContentToScrollBox(DrawObject *object)
{

    for(int i = 0; i < GetAmountOfSaves(); i++)
        object->scrollbox.text_content[i] = GetSaveNameFromSaveId(i + 1);

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
    object->scrollbox.box_click        = &UpdateSaveStatsText;
    object->scrollbox.text_content     = malloc(sizeof(char *) * GetAmountOfSaves());

    AddSaveContentToScrollBox(object);
    AddObjectToDrawLayer(object);

}

void InitializeLoadSaveMenu() 
{

    
    if (CreateNewDrawLayer() == -1) {

        Log("ERROR: LoadMenu could not create new draw layer");
        return;

    }

    AddMenuWithChildsToDrawLayer(GetMenuWithChildsFromJsonLayer("LoadSaveMenu"));

    SaveNameTextObject   = GetDrawObjectFromJsonLayer("LoadSaveMenuSaveNameText");
    PlayerNameTextObject = GetDrawObjectFromJsonLayer("LoadSaveMenuPlayerNameText");

    AddObjectToDrawLayer(SaveNameTextObject);
    AddObjectToDrawLayer(PlayerNameTextObject);
    DisplayLoadSaveScrollBox();
    
}

void InitializeNewSaveMenu() 
{

    
    if (CreateNewDrawLayer() == -1) {

        Log("ERROR: NewMenu could not create new draw layer");
        return;

    }

    AddMenuWithChildsToDrawLayer(GetMenuWithChildsFromJsonLayer("NewSaveMenu"));

}


//LoadSave Button Callbacks

void NewSaveMenu_BCB()
{

    if (CreateNewDrawLayer() == -1) {

        Log("Error: NewSaveButtonCallBack, could not create drawlayer");
        return;

    }
    AddMenuWithChildsToDrawLayer(GetMenuWithChildsFromJsonLayer("NewSaveMenu"));

}

void StartGame()
{

    ClearDrawLayers();
    StartSimulation();
    SwitchToLoadingScreen();

}

void LoadSaveMenuLoad_BCB()
{

    StartGame();

}

void LoadSaveMenuBack_BCB()
{

    ClearDrawLayers();
    InitializeMainMenu();

}

void DeleteSave_BCB()
{

    DeleteSave(SaveNameTextObject->text.content, PlayerNameTextObject->text.content);

}

void NewSaveMenuBack_BCB()
{

    ClearCurrentDrawLayer();
    
}

void CreateSave_BCB()
{
    
    char *save_name_in_text_box = GetTextFromTextBox("SaveNameTextBox");
    
    if (strlen(save_name_in_text_box) == 0) {

        Log("save_name_in_text_box has length of 0");
        //return;
    }

    char *player_name_in_text_box = GetTextFromTextBox("PlayerNameTextBox");
    if (strlen(player_name_in_text_box) == 0) {

        Log("player_name_in_text_box has length of 0");
        //return;
    }

    //CreateNewSave(save_name_in_text_box, player_name_in_text_box);

    StartGame();

}
