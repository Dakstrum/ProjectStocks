#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "game.h"
#include "account.h"

#include "dbsave.h"

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"

#include "text.h"
#include "button.h"
#include "scrollbox.h"
#include "popup.h"

#include "generalpurposemenus.h"
#include "drawlayerutils.h"

#include "portfolio.h"

static MenuWithChilds *players_menu      = NULL;

static DrawObject *player_money_textobject = NULL;
static DrawObject *player_date_textobject  = NULL; 

static DrawObject *player_title_textobject   = NULL;
static DrawObject *player_desc_textobject    = NULL;

static DrawObject *player_bitmap    = NULL;

static int current_button_idx = -1;

static DrawObject *playerone_networth_text   = NULL;
static DrawObject *playertwo_networth_text   = NULL;
static DrawObject *playerthree_networth_text = NULL;

static DrawObject *playerone_name_text   = NULL;
static DrawObject *playertwo_name_text   = NULL;
static DrawObject *playerthree_name_text = NULL;

void PlayersMenu_RenderLogic();

void PlayersMenu_InitTextAndButtons();
void PlayersMenu_InitPlayerBitmapAndText(char* player_title);
void PlayersMenu_Clean();

void PlayersMenu_InitPlayerNames();
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
    PlayersMenu_InitPlayerNames();
    PlayersMenu_InitPlayerScrollBox();

}

void PlayersMenu_RenderLogic()
{

    if (player_money_textobject == NULL)
        return;
    
    SetTextContentWithCommaFormat(player_money_textobject, "%'.2f", Account_GetMoney(Account_GetPlayerId()));
    SetTextContent(player_date_textobject,  "%s",   Game_GetDate());

    PlayersMenu_InitPlayerNetworth();

}

void PlayersMenu_InitTextAndButtons()
{

    player_money_textobject   = GetJSONObjectAndAddToDrawLayer("PlayersMenuAccountMoneyTextObject");
    player_date_textobject    = GetJSONObjectAndAddToDrawLayer("PlayersMenuAccountDateTextObject");

    playerone_networth_text   = GetJSONObjectAndAddToDrawLayer("PlayersMenunetworth1TextObject");
    playertwo_networth_text   = GetJSONObjectAndAddToDrawLayer("PlayersMenunetworth2TextObject");
    playerthree_networth_text = GetJSONObjectAndAddToDrawLayer("PlayersMenunetworth3TextObject");

    playerone_name_text       = GetJSONObjectAndAddToDrawLayer("PlayersMenuname1TextObject");
    playertwo_name_text       = GetJSONObjectAndAddToDrawLayer("PlayersMenuname2TextObject");
    playerthree_name_text     = GetJSONObjectAndAddToDrawLayer("PlayersMenuname3TextObject");

}

void PlayerMenu_ScrollBoxClick(char *scroll_box_content, unsigned short int index)
{

}

void PlayersMenu_InitPlayerScrollBox()
{

    DrawObject *playerone_scrollbox = NULL, *playertwo_scrollbox = NULL, *playerthree_scrollbox = NULL;
    DrawObject *player_scrollboxes[3] = {playerone_scrollbox, playertwo_scrollbox, playerthree_scrollbox};

    int player_scrollboxes_x[3] = {176, 761, 1340};

    for(int i = 0; i < 3; i++)
    {
        player_scrollboxes[i] = Scrollbox_Create();

        AddObjectToDrawLayer(CreateAIPlayerOwnedCompaniesScrollbox(player_scrollboxes_x[i], 420, &PlayerMenu_ScrollBoxClick));
    }

}

void PlayersMenu_InitPlayerNames()
{

    DrawObject *player_names_text[3] = {playerone_name_text, playertwo_name_text, playerthree_name_text};

    Vector *players = Account_GetPlayers();
    Player *players_temp = players->elements;

    for(int i = 0; i < 3; i++)
        SetTextContent(player_names_text[i],  "%s",   players_temp[i + 1].name);

}

void PlayersMenu_InitPlayerNetworth()
{

    DrawObject *player_networth_text[3] = {playerone_networth_text, playertwo_networth_text, playerthree_networth_text};

    Vector *players = Account_GetPlayers();
    Player *players_temp = players->elements;

    for(int i = 0; i < 3; i++)
        SetTextContentWithCommaFormat(player_networth_text[i],  "%'.2f",  portfolio_get_networth(players_temp[i + 1].player_id));


}

void PlayersMenu_Clean()
{

    player_money_textobject  = NULL;
    player_date_textobject   = NULL; 
    player_title_textobject    = NULL;
    player_desc_textobject     = NULL;

    player_bitmap    = NULL;

    playerone_networth_text   = NULL;
    playertwo_networth_text   = NULL;
    playerthree_networth_text = NULL;

    playerone_name_text   = NULL;
    playertwo_name_text   = NULL;
    playerthree_name_text = NULL;

    players_menu      = NULL;

    current_button_idx = -1;

}
