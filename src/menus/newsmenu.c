#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"
#include "text.h"
#include "account.h"
#include "generalpurposemenus.h"
#include "drawlayerutils.h"

static MenuWithChilds *news_menu = NULL;

static DrawObject *player_money_textobject = NULL;
static DrawObject *player_date_textobject  = NULL; 

void InitalizeNewsMenuText();
void NewsMenuRenderLogic();

void InitializeNewsMenu() 
{ 

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: StocksMenu could not create new draw layer");
        return;

    }
    
    news_menu = GetJSONMenuAndAddToDrawLayer("NewsMenu");

    InitalizeNewsMenuText();
    InitializeSpeedSelectObject("NewsMenu");

    NewsMenuRenderLogic();
}

void InitalizeNewsMenuText()
{

    player_money_textobject = GetJSONObjectAndAddToDrawLayer("NewsMenuAccountMoneyTextObject");
    player_date_textobject  = GetJSONObjectAndAddToDrawLayer("NewsMenuAccountDateTextObject");

}

void NewsMenuRenderLogic()
{

    if (player_money_textobject == NULL)
        return;
    
    SetTextContent(player_money_textobject, "%.2f", GetAccountMoney());
    SetTextContent(player_date_textobject,  "%s",   GetDate());

}

void CleanNewsMenu()
{

    player_money_textobject = NULL;
    player_date_textobject  = NULL;

    news_menu = NULL;
}
