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

static DrawObject *player_money_textobject = NULL;
static DrawObject *player_date_textobject  = NULL; 

static DrawObject *player_title_textobject   = NULL;
static DrawObject *player_desc_textobject    = NULL;

static DrawObject *player_bitmap    = NULL;

static int current_button_idx = -1;

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
    //PlayersMenu_InitPlayerNames();
    //PlayersMenu_InitPlayerScrollBox();
    //PlayersMenu_InitPlayerNetworth();

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

void PlayerMenu_ScrollBoxClick(char *scroll_box_content, unsigned short int index)
{

}

void PlayersMenu_InitPlayerScrollBox()
{

    DrawObject *playerone_scrollbox = NULL, *playertwo_scrollbox = NULL, *playerthree_scrollbox = NULL;
    DrawObject *player_scrollboxes[3] = {playerone_scrollbox, playertwo_scrollbox, playerthree_scrollbox};

    for(int i = 0; i < 3; i++)
    {
        player_scrollboxes[i] = Scrollbox_Create();

       // int x = PlayerMenu_GetCenteredPointForPlayerBoxScrollbox(i, 1400, player_scrollboxes);
       // AddObjectToDrawLayer(CreateAIPlayerOwnedCompaniesScrollbox(x, 560, &PlayerMenu_ScrollBoxClick));
    
    }

}


void PlayersMenu_InitPlayerNames()
{

    DrawObject *playerone_name_text   = GetJSONObjectAndAddToDrawLayer("PlayersMenuname1TextObject");
    DrawObject *playertwo_name_text   = GetJSONObjectAndAddToDrawLayer("PlayersMenuname2TextObject");
    DrawObject *playerthree_name_text = GetJSONObjectAndAddToDrawLayer("PlayersMenuname3TextObject");

    DrawObject *player_names[3] = {playerone_name_text, playertwo_name_text, playerthree_name_text};

    for(int i = 0; i < 3; i++)
    {

        player_names[i]->text.content   = "Kevin Shmider";
        AddObjectToDrawLayer(player_names[i]);
    
    }

}



void PlayersMenu_InitPlayerNetworth()
{


    DrawObject *playerone_networth_text   = GetJSONObjectAndAddToDrawLayer("PlayersMenunetworth1TextObject");
    DrawObject *playertwo_networth_text   = GetJSONObjectAndAddToDrawLayer("PlayersMenunetworth2TextObject");
    DrawObject *playerthree_networth_text = GetJSONObjectAndAddToDrawLayer("PlayersMenunetworth3TextObject");

    DrawObject *player_networth[3] = {playerone_networth_text, playertwo_networth_text, playerthree_networth_text};

    for(int i = 0; i < 3; i++)
    {
    
        player_networth[i]->text.content   = "1,023,233";
        AddObjectToDrawLayer(player_networth[i]);
    
    }

}

void PlayersMenu_Clean()
{

    player_money_textobject  = NULL;
    player_date_textobject   = NULL; 
    player_title_textobject    = NULL;
    player_desc_textobject     = NULL;

    player_bitmap    = NULL;

    players_menu      = NULL;

    current_button_idx = -1;

}
