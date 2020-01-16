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

static MenuWithChilds *cards_menu      = NULL;
static MenuWithChilds *apply_card_menu = NULL;

void AddCompanyContentToApplyCardScrollBox(DrawObject *object);

void InitializeCardsMenu() 
{

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: cards Menu could not create new draw layer");
        return;
    }

    cards_menu = GetMenuWithChildsFromJsonLayer("CardsMenu");
    AddMenuWithChildsToDrawLayer(cards_menu);

}

void LoadCompanyToApplyCardToScrollBoxClick(char *scroll_box_content)
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

void AddCompanyContentToApplyCardScrollBox(DrawObject *object)
{

    for(int i = 0; i < GetAmountOfCompanies(); i++)
        object->scrollbox.text_content[i]  = GetStockNameFromStockId(i+1);

}

void TempApplyButtonCallBack()
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

void CardApplyExitButtonCallBack()
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
