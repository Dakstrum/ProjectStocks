#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"

static MenuWithChilds *cards_menu = NULL;

void InitializeCardsMenu() 
{

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: cards Menu could not create new draw layer");
        return;
    }


    cards_menu = GetMenuWithChildsFromJsonLayer("CardsMenu");
    AddMenuWithChildsToDrawLayer(cards_menu);

}

void CleanUpCardsMenu() 
{

    if (cards_menu != NULL)
        free(cards_menu);

    cards_menu = NULL;
    
}

