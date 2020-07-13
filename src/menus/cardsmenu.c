#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include "simulation.h"
#include "scrollbox.h"
#include "text.h"
#include "generalpurposemenus.h"
#include "drawlayerutils.h"

static MenuWithChilds *cards_menu      = NULL;
static MenuWithChilds *apply_card_menu = NULL;

static DrawObject *player_money_textobject = NULL;
static DrawObject *player_date_textobject  = NULL; 

static DrawObject *card_title_textobject   = NULL;
static DrawObject *card_desc_textobject    = NULL;


void CardsMenuRenderLogic();

void InitalizePositiveCardsScrollBox();
void InitalizeNegativeCardsScrollBox();

void InitializeCardMenuCompanyScrollBox();

void InitializeCardsMenuText();

void InitializeCardsMenu() 
{

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: cards Menu could not create new draw layer");
        return;
    }

    cards_menu = GetJSONMenuAndAddToDrawLayer("CardsMenu");

    InitalizePositiveCardsScrollBox();
    InitalizeNegativeCardsScrollBox();

    InitializeCardsMenuText(); 

    InitializeSpeedSelectObject("CardsMenu");

    CardsMenuRenderLogic();

}

void CardsMenuRenderLogic()
{

    if (player_money_textobject == NULL)
        return;
    
    SetTextContent(player_money_textobject, "%.2f", GetAccountMoney());
    SetTextContent(player_date_textobject,  "%s",   GetDate());

}

void InitializeCardsMenuText()
{

    player_money_textobject = GetJSONObjectAndAddToDrawLayer("CardsMenuAccountMoneyTextObject");
    player_date_textobject  = GetJSONObjectAndAddToDrawLayer("CardsMenuAccountDateTextObject");
    card_title_textobject   = GetJSONObjectAndAddToDrawLayer("CardsMenuCardTitleTextObject");
    card_desc_textobject    = GetJSONObjectAndAddToDrawLayer("CardsMenuCardDescriptionTextObject");

}

void PopulateCardMenuCompanyScrollBox(DrawObject *object)
{

    for(int i = 0; i < GetNumCompanies(); i++)
        object->scrollbox.text_content[i]  = GetCompanyName(i+1);

}


void CardMenuCompanyScrollBoxClick(char *scroll_box_content, unsigned short int index)
{

    LogF("TODO: apply card to company");

}

void InitializeCardMenuCompanyScrollBox() 
{

    DrawObject *object = CreateScrollBoxObject();

    object->x          = 757;
    object->y          = 363;
    object->width      = 288;
    object->height     = 200;
    object->asset_path = "assets/images/companyicons/StocksBox.png";

    object->scrollbox.num_items    = GetNumCompanies();
    object->scrollbox.box_click    = &CardMenuCompanyScrollBoxClick;
    object->scrollbox.text_content = malloc(sizeof(char *) * GetNumCompanies());

    PopulateCardMenuCompanyScrollBox(object);
    AddObjectToDrawLayer(object);

}

void LoadCardClick(char *scroll_box_content, unsigned short int index)
{

    if (card_title_textobject == NULL || card_desc_textobject == NULL)
        return;

    SetTextContent(card_title_textobject, "%s", scroll_box_content);
    SetTextContent(card_desc_textobject,  "%s", scroll_box_content);

}

void InitalizePositiveCardsScrollBox() 
{

    DrawObject *object = CreateScrollBoxObject();

    object->x          = 50;
    object->y          = 233;
    object->width      = 288;
    object->height     = 200;
    object->asset_path = "assets/images/companyicons/StocksBox.png";

    object->scrollbox.num_items        = 2;
    object->scrollbox.box_click        = &LoadCardClick;
    object->scrollbox.text_content     = malloc(sizeof(char *) * 2);

    object->scrollbox.text_content[0]  = GetFormattedPointer("Fake Quarter Earnings");
    object->scrollbox.text_content[1]  = GetFormattedPointer("Level 1 Advertising");

    AddObjectToDrawLayer(object);

}

void InitalizeNegativeCardsScrollBox() 
{

    DrawObject *object = CreateScrollBoxObject();

    object->x          = 480;
    object->y          = 233;
    object->width      = 288;
    object->height     = 200;
    object->asset_path = "assets/images/companyicons/StocksBox.png";

    object->scrollbox.num_items        = 2;
    object->scrollbox.box_click        = &LoadCardClick;
    object->scrollbox.text_content     = malloc(sizeof(char *) * 2);

    object->scrollbox.text_content[0]  = GetFormattedPointer("Damaging Product");
    object->scrollbox.text_content[1]  = GetFormattedPointer("Level 1 Defamation");

    AddObjectToDrawLayer(object);

}

void ApplyMenu_BCB()
{
   
    if (apply_card_menu == NULL) {

        CreateNewDrawLayer();
        apply_card_menu = GetMenuWithChildsFromJsonLayer("ApplyCardMenu");
        AddMenuWithChildsToDrawLayer(apply_card_menu);
        InitializeCardMenuCompanyScrollBox(); 
        
    } else {

        ClearCurrentDrawLayer();
        apply_card_menu = NULL;

    }

}

void CleanCardsMenu()
{

    cards_menu      = NULL;
    apply_card_menu = NULL;

    player_money_textobject  = NULL;
    player_date_textobject   = NULL; 
    card_title_textobject    = NULL;
    card_desc_textobject     = NULL;

}
