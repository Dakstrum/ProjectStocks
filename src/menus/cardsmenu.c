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
#include "dbaccount.h"
#include "account.h"
#include "simulation.h"
#include "scrollbox.h"
#include "text.h"
#include "generalpurposemenus.h"

static MenuWithChilds *cards_menu         = NULL;
static MenuWithChilds *apply_card_menu    = NULL;

static DrawObject *AccountMoneyTextObject = NULL;
static DrawObject *AccountDateTextObject  = NULL; 

static DrawObject *CardTitleTextObject    = NULL;
static DrawObject *CardDescTextObject     = NULL;

void AddCompanyContentToApplyCardScrollBox(DrawObject *object);
void CardsMenuRenderLogic();

void DisplayPositiveCardsScrollBox();
void DisplayNegativeCardsScrollBox();
void DisplayCompanyToApplyCardToScrollBox();

void InitializeCardsMenu() 
{

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: cards Menu could not create new draw layer");
        return;
    }

    cards_menu = GetMenuWithChildsFromJsonLayer("CardsMenu");
    AddMenuWithChildsToDrawLayer(cards_menu);

    AccountMoneyTextObject = GetObjectAndDraw("StocksMenuAccountMoneyText");
    AccountDateTextObject  = GetObjectAndDraw("StocksMenuAccountDateText");
    CardTitleTextObject    = GetObjectAndDraw("CardTitleText");
    CardDescTextObject     = GetObjectAndDraw("CardDescText");

    CardsMenuRenderLogic();
    InitializeSpeedSelectObject();

    DisplayPositiveCardsScrollBox();
    DisplayNegativeCardsScrollBox();
}

void CardsMenuRenderLogic()
{

    if (AccountMoneyTextObject == NULL)
        return;
    
    SetTextContent(AccountMoneyTextObject, "%.2f", GetAccountMoney());
    SetTextContent(AccountDateTextObject,  "%s",   GetDate());

}

void AddCompanyContentToApplyCardScrollBox(DrawObject *object)
{

    for(int i = 0; i < GetAmountOfCompanies(); i++)
        object->scrollbox.text_content[i]  = GetStockNameFromStockId(i+1);

}

void TempApply_BCB()
{
	//TestCodePleaseKeep
	if (apply_card_menu == NULL) {

        CreateNewDrawLayer();
        apply_card_menu = GetMenuWithChildsFromJsonLayer("ApplyCardMenu");
        AddMenuWithChildsToDrawLayer(apply_card_menu);
        DisplayCompanyToApplyCardToScrollBox(); 
        
    } else {

        ClearCurrentDrawLayer();
        apply_card_menu = NULL;

    }

}

void CardApplyExit_BCB()
{
    if (apply_card_menu == NULL) {

        CreateNewDrawLayer();
        apply_card_menu = GetMenuWithChildsFromJsonLayer("ApplyCardMenu");
        AddMenuWithChildsToDrawLayer(apply_card_menu);
        DisplayCompanyToApplyCardToScrollBox(); 
        
    } else {

        ClearCurrentDrawLayer();
        apply_card_menu = NULL;

    }
}


void CleanUpCardsMenu() 
{

    if (cards_menu != NULL)
        free(cards_menu);

    cards_menu = NULL;
    
}

void LoadCompanyToApplyCardToScrollBoxClick(char *scroll_box_content, unsigned short int index)
{

    LogF("TODO: apply card to company");



}

void DisplayCompanyToApplyCardToScrollBox() 
{

    DrawObject *object = CreateScrollBoxObject();

    object->x          = 757;
    object->y          = 363;
    object->width      = 288;
    object->height     = 200;
    object->asset_path = "assets/images/companyicons/StocksBox.png";

    object->scrollbox.num_items        = GetAmountOfCompanies();
    object->scrollbox.box_click        = &LoadCompanyToApplyCardToScrollBoxClick;
    object->scrollbox.text_content     = malloc(sizeof(char *) * 2);

    AddCompanyContentToApplyCardScrollBox(object);
    AddObjectToDrawLayer(object);

}

void LoadNewActiveCard(char *scroll_box_content, unsigned short int index)
{

    SetTextContent(CardTitleTextObject, "%s", scroll_box_content);
    SetTextContent(CardDescTextObject, "%s", scroll_box_content);

}

void DisplayPositiveCardsScrollBox() 
{

    DrawObject *object = CreateScrollBoxObject();

    object->x          = 50;
    object->y          = 233;
    object->width      = 288;
    object->height     = 200;
    object->asset_path = "assets/images/companyicons/StocksBox.png";

    object->scrollbox.num_items        = 2;
    object->scrollbox.box_click        = &LoadNewActiveCard;
    object->scrollbox.text_content     = malloc(sizeof(char *) * 2);

    object->scrollbox.text_content[0]  = GetFormattedPointer("Fake Quarter Earnings");
    object->scrollbox.text_content[1]  = GetFormattedPointer("Level 1 Advertising");

    AddObjectToDrawLayer(object);

}

void DisplayNegativeCardsScrollBox() 
{

    DrawObject *object = CreateScrollBoxObject();

    object->x          = 480;
    object->y          = 233;
    object->width      = 288;
    object->height     = 200;
    object->asset_path = "assets/images/companyicons/StocksBox.png";

    object->scrollbox.num_items        = 2;
    object->scrollbox.box_click        = &LoadNewActiveCard;
    object->scrollbox.text_content     = malloc(sizeof(char *) * 2);

    object->scrollbox.text_content[0]  = GetFormattedPointer("Damaging Product");
    object->scrollbox.text_content[1]  = GetFormattedPointer("Level 1 Defamation");

    AddObjectToDrawLayer(object);

}
