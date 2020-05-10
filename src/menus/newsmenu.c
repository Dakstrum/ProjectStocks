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

static DrawObject *AccountMoneyTextObject = NULL;
static DrawObject *AccountDateTextObject  = NULL; 

void NewsMenuRenderLogic();

void InitializeNewsMenu() 
{ 

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: StocksMenu could not create new draw layer");
        return;

    }
    
    AddMenuWithChildsToDrawLayer(GetMenuWithChildsFromJsonLayer("NewsMenu"));

    AccountMoneyTextObject = GetObjectAndDraw("StocksMenuAccountMoneyText");
    AccountDateTextObject  = GetObjectAndDraw("StocksMenuAccountDateText");
    
    NewsMenuRenderLogic();
    InitializeSpeedSelectObject();
}

void NewsMenuRenderLogic()
{

    if (AccountMoneyTextObject == NULL)
        return;
    
    SetTextContent(AccountMoneyTextObject, "%.2f", GetAccountMoney());
    SetTextContent(AccountDateTextObject,  "%s",   GetDate());

}

