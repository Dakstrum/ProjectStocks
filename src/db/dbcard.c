#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "shared.h"
#include "dbutils.h"
#include "dbcard.h"
#include "log.h"
#include "account.h"

static Vector *cards        = NULL;
static Vector *player_cards = NULL;


//Cards
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

char* GetCardTitle(int card_id)
{


    Card *temp = (Card *)cards->elements;
    for (size_t i = 0; i < cards->num_elements; i++)
        if (temp[i].card_id == (unsigned int)card_id)
            return temp[i].card_name;

    return 0;

}

char* GetCardDescription(char* card_title)
{

	assert(card_title != NULL);

    Card *temp = (Card *)cards->elements;
    for (size_t i = 0; i < cards->num_elements;i++)
        if (strcmp(temp[i].card_name, card_title) == 0)
            return temp[i].card_desc;

    return 0;

}

char* GetCardPath(char* card_title)
{

    assert(card_title != NULL);

    Card *temp = (Card *)cards->elements;
    for (size_t i = 0; i < cards->num_elements;i++)
        if (strcmp(temp[i].card_name, card_title) == 0)
            return temp[i].card_path;

    return 0;

}

int GetCardPriceModifier(char* card_title)
{

    assert(card_title != NULL);

    Card *temp = (Card *)cards->elements;
    for (size_t i = 0; i < cards->num_elements; i++)
        if (strcmp(temp[i].card_name, card_title) == 0)
            return temp[i].price_modifier;

    return 0;

}

float GetCardModifierLength(char* card_title)
{

    assert(card_title != NULL);

    Card *temp = (Card *)cards->elements;
    for (size_t i = 0; i < cards->num_elements; i++)
        if (strcmp(temp[i].card_name, card_title) == 0)
            return temp[i].modifier_length;

    return 0;

}

int Card_Callback(void *card, int argc, char **argv, char **col_name) 
{

    if (argc == 0)
        return 0;

    Card temp;

    temp.card_id = atoi(argv[0]);
    strncpy(temp.card_name, argv[1], 32);
    strncpy(temp.card_desc, argv[2], 255);
    strncpy(temp.card_path, argv[3], 255);
    temp.price_modifier = atof(argv[4]);

    temp.modifier_length = atoi(argv[5]);

    Vector_PushBack(cards, &temp);

    return 0;

}

int GetCardType(char* card_title)
{

    if(GetCardPriceModifier(card_title) > 0)
        return 1;
    else
        return 0;


}

void InitializeCards()
{

    cards = Vector_Create(sizeof(Card), 4);
    ExecuteQueryF(&Card_Callback, NULL, "SELECT C.CardId, C.CardName, C.CardDesc, C.CardPath, C.PriceModifier, C.ModifierLength FROM Cards C");

}

//PlayerCards
int PlayerCard_Callback(void *card, int argc, char **argv, char **col_name) 
{

    if (argc == 0)
        return 0;

    PlayerCard temp;

    temp.player_card_id = atoi(argv[0]);
    temp.player_id      = atoi(argv[1]);
    temp.card_id        = atoi(argv[2]);

    Vector_PushBack(player_cards, &temp);

    return 0;

}

void InitializePlayerCards()
{

    player_cards = Vector_Create(sizeof(PlayerCard), 4);
    ExecuteQueryF(&PlayerCard_Callback, NULL, "SELECT C.PlayerCardId, C.PlayerId, C.CardId FROM PlayerCards C");

}

void AddCardToPlayer(int card_id)
{

    sqlite3 *db;
    if (OpenConnection(&db, DefaultConnection()) == 0)
        ExecuteQuery(GetFormattedPointer("INSERT INTO PlayerCards (PlayerId, CardId) VALUES (%d, %d)", GetCurrentPlayerId(), card_id), NULL, NULL, db);

    sqlite3_close(db);

}

void RemoveCardToPlayer()
{



}



int GetNumOfPlayerCards()
{

    assert(cards != NULL);
    return player_cards->num_elements;

}



Card *GetAllPlayerCards()
{

    assert(cards != NULL);
    return (Card *)player_cards->elements;

}

int GetNumOfPlayerPositiveCards()
{
    int amount = 0;

    PlayerCard *temp = (PlayerCard *)player_cards->elements;

    for(size_t i = 0; i < player_cards->num_elements; i++)  
        if(GetCardType(GetCardTitle(temp[i].card_id)))
            amount++;

    return amount;


}

int GetNumOfPlayerNegativeCards()
{

    int amount = 0;

    PlayerCard *temp = (PlayerCard *)player_cards->elements;
    
    for(size_t i = 0; i > player_cards->num_elements; i++)  
        if(!GetCardType(GetCardTitle(temp[i].card_id)))
            amount++;

    return amount;
 
}