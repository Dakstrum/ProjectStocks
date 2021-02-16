#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"
#include "cache.h"
#include "graph.h"
#include "startup.h"
#include "dbcompany.h"
#include "dbaccount.h"
#include "account.h"
#include "game.h"
#include "simulation.h"
#include "scrollbox.h"
#include "text.h"
#include "generalpurposemenus.h"
#include "drawlayerutils.h"
#include "popup.h"
#include "button.h"

static MenuWithChilds *players_menu      = NULL;
static MenuWithChilds *apply_player_menu = NULL;

static DrawObject *player_money_textobject = NULL;
static DrawObject *player_date_textobject  = NULL; 

static DrawObject *player_title_textobject   = NULL;
static DrawObject *player_desc_textobject    = NULL;

static DrawObject *player_bitmap    = NULL;

static int current_button_idx = -1;

int num_of_players = 3; // TEMP//This will be replaced with a get function when AI is figured out.

void PlayersMenu_RenderLogic();

void PlayersMenu_InitTextAndButtons();
void PlayersMenu_InitPlayerBitmapAndText(char* player_title);
void PlayersMenu_Clean();

int GetCenteredPointForPlayerBox(int i);
int GetCenteredPointForPlayerNameText(int i);
int PlayerMenu_GetCenteredPointForPlayerBoxObject(int i, int centered_point, DrawObject *drawobject_array[]);
int PlayerMenu_GetCenteredPointForPlayerBoxScrollbox(int i, int centered_point, DrawObject *drawobject_array[]);

void PlayersMenu_InitPlayerBoxes();
void PlayersMenu_InitPlayerNames();
void PlayersMenu_InitPlayerNetworthLabel();
void PlayersMenu_InitPlayerAvailableCardsLabel();
void PlayersMenu_InitPlayerScrollBox();
void PlayersMenu_InitPlayerNetworth();


void PlayersMenu_Init() 
{

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: players Menu could not create new draw layer");
        return;
    }

    players_menu = GetJSONMenuAndAddToDrawLayer("PlayersMenu");

    GeneralPurposeMenus_InitSpeedSelectObject("PlayersMenu");
    PlayersMenu_InitTextAndButtons();
    PlayersMenu_InitPlayerBoxes();
    PlayersMenu_InitPlayerNames();
    PlayersMenu_InitPlayerNetworthLabel();
    PlayersMenu_InitPlayerAvailableCardsLabel();
    PlayersMenu_InitPlayerScrollBox();
    PlayersMenu_InitPlayerNetworth();

}

void PlayersMenu_RenderLogic()
{

    if (player_money_textobject == NULL)
        return;
    
    SetTextContent(player_money_textobject, "%.2f", Account_GetMoney());
    SetTextContent(player_date_textobject,  "%s",   Game_GetDate());

}

void PlayersMenu_InitTextAndButtons()
{
    player_money_textobject  = GetJSONObjectAndAddToDrawLayer("PlayersMenuAccountMoneyTextObject");
    player_date_textobject   = GetJSONObjectAndAddToDrawLayer("PlayersMenuAccountDateTextObject");
}

void PlayersMenu_InitPlayerBoxes()
{ 
    

    DrawObject *playerone_box = NULL, *playertwo_box = NULL, *playerthree_box = NULL;
    DrawObject *player_boxes[3] = {playerone_box, playertwo_box, playerthree_box};

    for(int i = 0; i < num_of_players; i++)
    {

        player_boxes[i]         = CreateNewDrawObject();
        player_boxes[i]->type   = MENU;
        player_boxes[i]->x      = PlayerMenu_GetCenteredPointForPlayerBoxObject(i, 760, player_boxes);
        player_boxes[i]->y      = 362;
        player_boxes[i]->width  = 438;
        player_boxes[i]->height = 629;

        player_boxes[i]->asset_path = "assets/images/playersmenu/player-box.png";

        AddObjectToDrawLayer(player_boxes[i]);
    
    }

}

void PlayerMenu_ScrollBoxClick(char *scroll_box_content, unsigned short int index)
{

}

void PlayersMenu_InitPlayerScrollBox()
{

    DrawObject *playerone_scrollbox = NULL, *playertwo_scrollbox = NULL, *playerthree_scrollbox = NULL;
    DrawObject *player_scrollboxes[3] = {playerone_scrollbox, playertwo_scrollbox, playerthree_scrollbox};

    for(int i = 0; i < num_of_players; i++)
    {
        player_scrollboxes[i] = Scrollbox_Create();

        int x = PlayerMenu_GetCenteredPointForPlayerBoxScrollbox(i, 1400, player_scrollboxes);
        AddObjectToDrawLayer(CreateAIPlayerOwnedCompaniesScrollbox(x, 560, &PlayerMenu_ScrollBoxClick));
    
    }

}


void PlayersMenu_InitPlayerNames()
{

    DrawObject *playerone_name_text = NULL, *playertwo_name_text = NULL, *playerthree_name_text = NULL;
    DrawObject *player_names[3] = {playerone_name_text, playertwo_name_text, playerthree_name_text};

    for(int i = 0; i < num_of_players; i++)
    {

        player_names[i]                   = Text_Create();
        player_names[i]->width            = 564;
        player_names[i]->x                = PlayerMenu_GetCenteredPointForPlayerBoxObject(i, 778, player_names);
        player_names[i]->y                = 360;
        
        player_names[i]->text.content   = "Kevin Shmider";
        player_names[i]->text.font_size = 45;

        AddObjectToDrawLayer(player_names[i]);
    
    }

}

void PlayersMenu_InitPlayerNetworthLabel()
{

    DrawObject *playerone_networth_label_text = NULL, *playertwo_networth_label_text = NULL, *playerthree_networth_label_text = NULL;
    DrawObject *player_networth_label[3] = {playerone_networth_label_text, playertwo_networth_label_text, playerthree_networth_label_text};

    for(int i = 0; i < num_of_players; i++)
    {

        player_networth_label[i]                   = Text_Create();
        player_networth_label[i]->width            = 564;
        player_networth_label[i]->x                = PlayerMenu_GetCenteredPointForPlayerBoxObject(i, 778, player_networth_label);
        player_networth_label[i]->y                = 440;
        
        player_networth_label[i]->text.content   = "Net Worth:";
        player_networth_label[i]->text.font_size = 40;

        AddObjectToDrawLayer(player_networth_label[i]);
    
    }

}


void PlayersMenu_InitPlayerNetworth()
{

    DrawObject *playerone_networth_text = NULL, *playertwo_networth_text = NULL, *playerthree_networth_text = NULL;
    DrawObject *player_networth[3] = {playerone_networth_text, playertwo_networth_text, playerthree_networth_text};

    for(int i = 0; i < num_of_players; i++)
    {

        player_networth[i]                   = Text_Create();
        player_networth[i]->width            = 564;
        player_networth[i]->x                = PlayerMenu_GetCenteredPointForPlayerBoxObject(i, 935, player_networth);
        player_networth[i]->y                = 440;
        
        player_networth[i]->text.content   = "1,023,233";
        player_networth[i]->text.font_size = 40;

        AddObjectToDrawLayer(player_networth[i]);
    
    }

}

void PlayersMenu_InitPlayerAvailableCardsLabel()
{

    DrawObject *playerone_available_cards_label_text = NULL, *playertwo_available_cards_label_text = NULL, *playerthree_available_cards_label_text = NULL;
    DrawObject *player_available_cards_label[3] = {playerone_available_cards_label_text, playertwo_available_cards_label_text, playerthree_available_cards_label_text};

    for(int i = 0; i < num_of_players; i++)
    {

        player_available_cards_label[i]                   = Text_Create();
        player_available_cards_label[i]->width            = 564;
        player_available_cards_label[i]->x                = PlayerMenu_GetCenteredPointForPlayerBoxObject(i, 778, player_available_cards_label);
        player_available_cards_label[i]->y                = 480;
        
        player_available_cards_label[i]->text.content   = "Available Cards:";
        player_available_cards_label[i]->text.font_size = 40;

        AddObjectToDrawLayer(player_available_cards_label[i]);
    
    }

}

int PlayerMenu_GetCenteredPointForPlayerBoxScrollbox(int i, int centered_point, DrawObject *drawobject_array[])
{

    switch(i) {

        case 0 :
            return centered_point;

        case 1 :
            drawobject_array[0]->x = drawobject_array[0]->x + 300;
            return (centered_point - 630);

        case 2 :
            drawobject_array[0]->x = drawobject_array[0]->x + 300;
            drawobject_array[1]->x = drawobject_array[1]->x + 300;
            return (centered_point - (630 * 2));


    }
    
    return 0;

}

int PlayerMenu_GetCenteredPointForPlayerBoxObject(int i, int centered_point, DrawObject *drawobject_array[])
{

    switch(i) {

        case 0 :
            return centered_point;

        case 1 :
            drawobject_array[0]->x = drawobject_array[0]->x + 300;
            return (centered_point - 330);

        case 2 :
            drawobject_array[0]->x = drawobject_array[0]->x + 300;
            drawobject_array[1]->x = drawobject_array[1]->x + 300;
            return (centered_point - (330 * 2));


    }
    
    return 0;

}

void PlayersMenu_Clean()
{

    player_money_textobject  = NULL;
    player_date_textobject   = NULL; 
    player_title_textobject    = NULL;
    player_desc_textobject     = NULL;

    player_bitmap    = NULL;

    players_menu      = NULL;
    apply_player_menu = NULL;

    current_button_idx = -1;

}
