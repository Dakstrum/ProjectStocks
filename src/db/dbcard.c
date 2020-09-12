#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "shared.h"
#include "dbutils.h"
#include "dbcard.h"
#include "log.h"

static Vector *cards = NULL;

unsigned int GetCardId(char* card_title)
{

	assert(card_title != NULL);
    LogF("%s", card_title);
    Card *temp = (Card *)cards->elements;
    for (size_t i = 0; i < cards->num_elements; i++)
    	if (strcmp(temp[i].card_name, card_title) == 0)
            return temp[i].card_id;

    return 0;

}

char* GetCardDescription(char* card_title)
{


	return;

}

char* GetCardPath(char* card_title)
{


	return;

}

int GetCardPriceModifier(char* card_title)
{


	return;
}

int GetCardModifierLength(char* card_title)
{


	return;
}

int Card_Callback(void *card, int argc, char **argv, char **col_name) 
{

    if (argc == 0)
        return 0;

    Card temp;

    temp.card_id = atoi(argv[0]);
    strncpy(temp.card_name, argv[1], 32);
    strncpy(temp.card_path, argv[2], 255);
    temp.price_modifier = atof(argv[3]);

    temp.modifier_length = atoi(argv[4]);

    Vector_PushBack(cards, &temp);

    return 0;

}

void InitializeCards()
{

    cards = Vector_Create(sizeof(Card), 4);
    ExecuteQueryF(&Card_Callback, NULL, "SELECT C.CardId, C.CardName, C.CardPath, C.PriceModifier, C.ModifierLength FROM Cards C");

}
