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
#include "textbox.h"

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
#include "popup.h"

static MenuWithChilds *load_save_menu = NULL;
static MenuWithChilds *new_save_menu  = NULL;

static DrawObject *save_name_textobject   = NULL;
static DrawObject *player_name_textobject = NULL;
static DrawObject *saves_scrollbox        = NULL;

static Vector *saves          = NULL;
static int current_button_idx = -1;

void InitalizeSaveScrollBox();
void InitializeLoadSaveMenuText();
void InitializeNewSaveTextBoxes();

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

    save_name_textobject   = GetJSONObjectAndAddToDrawLayer("LoadSaveMenuSaveNameTextObject");
    player_name_textobject = GetJSONObjectAndAddToDrawLayer("LoadSaveMenuPlayerNameTextObject");

}

void SetSaveInfoText(char *save_name, char *player_name) 
{
    SetTextContent(save_name_textobject, "%s",   save_name);
    SetTextContent(player_name_textobject, "%s", player_name);
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
    InitializeNewSaveTextBoxes();

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
    Vector_Delete(saves);

    saves_scrollbox = NULL;
    InitalizeSaveScrollBox();

    current_button_idx = -1;

}

void CreateSave_BCB()
{

    char *save_name_in_text_box = GetTextFromTextBox("SaveNameTextBox");
    char *player_name_in_text_box = GetTextFromTextBox("PlayerNameTextBox");
    
    if (strlen(save_name_in_text_box) == 0) {

        DisplayPopupOnDrawLayer("Save Name is Necessary", "assets/images/generalpurposemenus/popups/yellowpopup.png");
        return;
    }

    if (strlen(player_name_in_text_box) == 0) {

        DisplayPopupOnDrawLayer("Plyaer Name is Necessary", "assets/images/generalpurposemenus/popups/yellowpopup.png");
        return;
    }
    CreateNewSave(save_name_in_text_box, player_name_in_text_box);
    StartGame();
    

}

void NewSaveMenuBack_BCB()
{

    ClearCurrentDrawLayer();
    
}

void SelectSaveClick(char *save_name, unsigned short int index)
{

    current_button_idx = index;
    PlayerSave *temp   = (PlayerSave *)saves->elements;
    SetSaveInfoText(save_name, temp[index].save_player_name);

}

void PopulateSaveScrollBox(DrawObject *object)
{

    PlayerSave *temp = (PlayerSave *)saves->elements;
    for(size_t i = 0; i < saves->num_elements; i++) {

        ScrollboxText text = {30, 5, NULL, 40, temp[i].save_name};
        Vector_PushBack(object->scrollbox.text_content[i], &text);

    }

}

void InitalizeSaveScrollBox() 
{

    saves           = GetAllSaves();
    saves_scrollbox = Scrollbox_Create();

    saves_scrollbox->x          = 537;
    saves_scrollbox->y          = 226;
    saves_scrollbox->width      = 288;
    saves_scrollbox->height     = 603;
    saves_scrollbox->scrollbox.vertical_spacing = 69;
    saves_scrollbox->asset_path = "assets/images/scrollbox/SaveBox.png";

    saves_scrollbox->scrollbox.num_items    = saves->num_elements;
    saves_scrollbox->scrollbox.box_click    = &SelectSaveClick;

    InitScrollboxVectors(saves_scrollbox);
    PopulateSaveScrollBox(saves_scrollbox);
    AddObjectToDrawLayer(saves_scrollbox);

}

void InitializeNewSaveTextBoxes()
{

    DrawObject *savename_tb = CreateTextBoxObject("SaveNameTextBox", "", 10, TEXTBOX_ACCEPT_ALPHABET_CHARACTERS | TEXTBOX_ACCEPT_NUMBER_CHARACTERS);
    savename_tb->x          = 28 + 515;
    savename_tb->y          = 96 + 179;
    savename_tb->width      = 145;
    savename_tb->height     = 22;

    DrawObject *playername_tb = CreateTextBoxObject("PlayerNameTextBox", "", 10, TEXTBOX_ACCEPT_ALPHABET_CHARACTERS | TEXTBOX_ACCEPT_NUMBER_CHARACTERS);
    playername_tb->x          = 28 + 515;
    playername_tb->y          = 198 + 179;
    playername_tb->width      = 145;
    playername_tb->height     = 22;

    AddObjectToDrawLayer(savename_tb);
    AddObjectToDrawLayer(playername_tb);

}

void CleanSaveMenu()
{

    if (saves != NULL)
        Vector_Delete(saves);

    load_save_menu         = NULL;
    new_save_menu          = NULL;
    save_name_textobject   = NULL;
    player_name_textobject = NULL;
    saves_scrollbox        = NULL;
    saves                  = NULL;
    current_button_idx     = -1;

}
