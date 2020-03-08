#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"
#include "text.h"

void NewsMenuRenderLogic();

static DrawObject *AccountMoneyTextObject    = NULL;

void InitializeNewsMenu() 
{ 

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: StocksMenu could not create new draw layer");
        return;

    }
    
    AddMenuWithChildsToDrawLayer(GetMenuWithChildsFromJsonLayer("NewsMenu"));

    AccountMoneyTextObject = GetDrawObjectFromJsonLayer("StocksMenuAccountMoneyText");
    AddObjectToDrawLayer(AccountMoneyTextObject);

    NewsMenuRenderLogic();
}

void NewsMenuRenderLogic()
{

    if (AccountMoneyTextObject == NULL)
        return;
    
    SetTextContent(AccountMoneyTextObject, "1,900");

}

