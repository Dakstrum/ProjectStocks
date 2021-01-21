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



DrawObject *player_boxes[3];
DrawObject *player_names[3];

void PlayersMenu_RenderLogic();

void PlayersMenu_InitTextAndButtons();
void PlayersMenu_InitPlayerBitmapAndText(char* player_title);
void PlayersMenu_Clean();
void PlayersMenu_InitPlayerBoxes();
void PlayersMenu_InitPlayerNames();
int GetCenteredPointForPlayerBox(int i);
int GetCenteredPointForPlayerNameText(int i);


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

    int num_of_players = 1; //This will be replaced with a get function when AI is figured out.

    DrawObject *playerone_box   = NULL;
    DrawObject *playertwo_box   = NULL;
    DrawObject *playerthree_box = NULL;

    player_boxes[0] = playerone_box;
    player_boxes[1] = playertwo_box;
    player_boxes[2] = playerthree_box;

    for(int i = 0; i < num_of_players; i++)
    {

        player_boxes[i]         = CreateNewDrawObject();
        player_boxes[i]->type   = MENU;
        player_boxes[i]->x      = GetCenteredPointForPlayerBox(i);
        player_boxes[i]->y      = 362;
        player_boxes[i]->width  = 564;
        player_boxes[i]->height = 357;

        player_boxes[i]->asset_path = "assets/images/playersmenu/player-box.png";

        AddObjectToDrawLayer(player_boxes[i]);
    
    }

}

int GetCenteredPointForPlayerBox(int i)
{
    int centered_box_x = 710;

    switch(i) {

        case 0 :
            return centered_box_x;

        case 1 :
            player_boxes[0]->x = player_boxes[0]->x + 300;
            return (centered_box_x - 330);

        case 2 :
            player_boxes[0]->x = player_boxes[0]->x + 300;
            player_boxes[1]->x = player_boxes[1]->x + 300;
            return (centered_box_x - (330 * 2));


    }
    
    return 0;

}

void PlayersMenu_InitPlayerNames()
{

    int num_of_players = 1; //This will be replaced with a get function when AI is figured out.

    DrawObject *playerone_name_text   = NULL;
    DrawObject *playertwo_name_text   = NULL;
    DrawObject *playerthree_name_text = NULL;

    player_names[0] = playerone_name_text;
    player_names[1] = playertwo_name_text;
    player_names[2] = playerthree_name_text;

    for(int i = 0; i < num_of_players; i++)
    {

        player_names[i]                   = Text_Create();
        player_names[i]->width            = 564;
        player_names[i]->x                = GetCenteredPointForPlayerNameText(i);
        player_names[i]->y                = 362;
        //player_names[i]->text.bitmap_path = "assets/images/playersmenu/player-box.png";
        
        player_names[i]->text.content   = "Kevin Shmider";

        AddObjectToDrawLayer(player_names[i]);
    
    }

}

int GetCenteredPointForPlayerNameText(int i)
{
    int centered_text_x = 738;

    switch(i) {

        case 0 :
            return centered_text_x;

        case 1 :
            player_names[0]->x = player_names[0]->x + 300;
            return (centered_text_x - 330);

        case 2 :
            player_names[0]->x = player_names[0]->x + 300;
            player_names[1]->x = player_names[1]->x + 300;
            return (centered_text_x - (330 * 2));


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
