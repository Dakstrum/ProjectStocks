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

static DrawObject *AccountMoneyTextObject    = NULL;

void NewsMenuRenderLogic();

void InitializeNewsMenu() 
{ 

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: StocksMenu could not create new draw layer");
        return;

    }
    
    AddMenuWithChildsToDrawLayer(GetMenuWithChildsFromJsonLayer("NewsMenu"));

    AccountMoneyTextObject = GetObjectAndDraw("StocksMenuAccountMoneyText");
    
    NewsMenuRenderLogic();
}

void NewsMenuRenderLogic()
{

    if (AccountMoneyTextObject == NULL)
        return;
    
    SetTextContent(AccountMoneyTextObject, "%.2f", account_money);

}

