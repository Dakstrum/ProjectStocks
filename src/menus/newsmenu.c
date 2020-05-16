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

static DrawObject *account_money_textobject = NULL;
static DrawObject *account_date_textobject  = NULL; 

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
    InitializeSpeedSelectObject();

    NewsMenuRenderLogic();
}

void InitalizeNewsMenuText()
{

    account_money_textobject = GetJSONObjectAndAddToDrawLayer("StocksMenuAccountMoneyText");
    account_date_textobject  = GetJSONObjectAndAddToDrawLayer("StocksMenuAccountDateText");

}

void NewsMenuRenderLogic()
{

    if (account_money_textobject == NULL)
        return;
    
    SetTextContent(account_money_textobject, "%.2f", GetAccountMoney());
    SetTextContent(account_date_textobject,  "%s",   GetDate());

}

void CleanNewsMenu()
{

    account_money_textobject = NULL;
    account_date_textobject  = NULL;

}
