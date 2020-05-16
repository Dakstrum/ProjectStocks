#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "text.h"
#include "jsonlayer.h"
#include "drawlayers.h"
#include "drawobject.h"
#include "scrollbox.h"

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
#include "dbsave.h"
#include "vector.h"
#include "drawlayerutils.h"

static MenuWithChilds *load_save_menu = NULL;
static MenuWithChilds *new_save_menu  = NULL;

static DrawObject *save_name_textobject   = NULL;
static DrawObject *player_name_textobject = NULL;
static DrawObject *saves_scrollbox        = NULL;

static Vector *saves          = NULL;
static int current_button_idx = -1;

void InitalizeSaveScrollBox();
void InitializeLoadSaveMenuText();

void InitializeLoadSaveMenu() 
{

    if (CreateNewDrawLayer() == -1) {

        Log("ERROR: LoadMenu could not create new draw layer");
        return;

    }

    load_save_menu = GetJSONMenuAndAddToDrawLayer("LoadSaveMenu");

    InitializeLoadSaveMenuText();
    InitalizeSaveScrollBox();
    
}

void InitializeLoadSaveMenuText()
{

    save_name_textobject   = GetJSONObjectAndAddToDrawLayer("LoadSaveMenuSaveNameText");
    player_name_textobject = GetJSONObjectAndAddToDrawLayer("LoadSaveMenuPlayerNameText");

}

void InitializeNewSaveMenu() 
{

    
    if (CreateNewDrawLayer() == -1) {

        Log("ERROR: NewMenu could not create new draw layer");
        return;

    }

    new_save_menu = GetJSONMenuAndAddToDrawLayer("NewSaveMenu");
    
}

void SetSaveInfoText(char *save_name, char *player_name) 
{
    SetTextContent(save_name_textobject, "%s",   save_name);
    SetTextContent(player_name_textobject, "%s", player_name);
}

void PopulateSaveScrollBox(DrawObject *object)
{
    PlayerSave *temp = (PlayerSave *)saves->elements;
    for(int i = 0; i < saves->num_elements; i++)
        object->scrollbox.text_content[i] = temp[i].save_name;

}

void SelectSaveClick(char *save_name, unsigned short int index)
{

    current_button_idx = index;
    PlayerSave *temp   = (PlayerSave *)saves->elements;
    SetSaveInfoText(save_name, temp[index].save_player_name);

}

void InitalizeSaveScrollBox() 
{

    saves           = GetAllSaves();
    saves_scrollbox = CreateScrollBoxObject();

    saves_scrollbox->type       = SCROLLBOX;
    saves_scrollbox->x          = 535;
    saves_scrollbox->y          = 226;
    saves_scrollbox->width      = 288;
    saves_scrollbox->height     = 603;
    saves_scrollbox->asset_path = "assets/images/companyicons/SaveBox.png";

    saves_scrollbox->scrollbox.num_items    = saves->num_elements;
    saves_scrollbox->scrollbox.box_click    = &SelectSaveClick;
    saves_scrollbox->scrollbox.text_content = malloc(sizeof(char *) * saves->num_elements);

    PopulateSaveScrollBox(saves_scrollbox);
    AddObjectToDrawLayer(saves_scrollbox);

}

void StartGame()
{

    SetInGameStatus(1);
    ClearDrawLayers();
    StartSimulation();
    SwitchToLoadingScreen();

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

void LoadSaveMenuLoad_BCB()
{

    if (current_button_idx == -1) {
        Log("No save selected");
        return;
    }

    PlayerSave *temp = (PlayerSave *)saves->elements;
    LoadSave(temp[current_button_idx].save_id, temp[current_button_idx].save_player_id);
    StartGame();

}

void LoadSaveMenuBack_BCB()
{

    ClearDrawLayers();
    InitializeMainMenu();

}

void DeleteSave_BCB()
{

    if (current_button_idx == -1)
        return;

    PlayerSave *temp = (PlayerSave *)saves->elements;
    DeleteSave(temp[current_button_idx].save_id);
    RemoveDrawObject(saves_scrollbox);

    SetSaveInfoText("", "");
    DeleteVector(saves);

    saves_scrollbox = NULL;
    InitalizeSaveScrollBox();

}

void CreateSave_BCB()
{

    char *save_name_in_text_box = GetTextFromTextBox("SaveNameTextBox");
    
    if (strlen(save_name_in_text_box) == 0) {

        Log("save_name_in_text_box has length of 0");
        return;
    }

    char *player_name_in_text_box = GetTextFromTextBox("PlayerNameTextBox");
    if (strlen(player_name_in_text_box) == 0) {

        Log("player_name_in_text_box has length of 0");
        // TODO setup popups when textboxes have no value.
        return;
    }
    CreateNewSave(save_name_in_text_box, player_name_in_text_box);
    StartGame();

}

void NewSaveMenuBack_BCB()
{

    ClearCurrentDrawLayer();
    
}

void CleanSaveMenu()
{

    if (saves != NULL)
        DeleteVector(saves);

    save_name_textobject   = NULL;
    player_name_textobject = NULL;
    saves_scrollbox        = NULL;
    saves                  = NULL;
    current_button_idx     = -1;

}
