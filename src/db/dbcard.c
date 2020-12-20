#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "queue.h"
#include "shared.h"
#include "dbutils.h"
#include "dbcard.h"
#include "log.h"
#include "account.h"
#include "game.h"

static Vector *cards        = NULL;
static Vector *player_cards = NULL;
static Vector *played_cards = NULL;

static Queue *card_queue   = NULL;

//Cards
int GetCardId(char* card_title)
{

	assert(card_title != NULL);

    Card *temp = (Card *)cards->elements;
    for (size_t i = 0; i < cards->num_elements; i++)
    	if (strcmp(temp[i].card_name, card_title) == 0)
            return temp[i].card_id;

    return 0;

}

char* GetCardTitle(unsigned int card_id)
{


    Card *temp = (Card *)cards->elements;
    for (size_t i = 0; i < cards->num_elements; i++)
        if (temp[i].card_id == (unsigned int)card_id)
            return temp[i].card_name;

    return 0;

}

char* GetCardDescription(unsigned int card_id)
{

    Card *temp = (Card *)cards->elements;
    for (size_t i = 0; i < cards->num_elements;i++)
        if (temp[i].card_id == (unsigned int)card_id)
            return temp[i].card_desc;

    return 0;

}

char* GetCardPath(unsigned int card_id)
{

    Card *temp = (Card *)cards->elements;
    for (size_t i = 0; i < cards->num_elements;i++)
        if (temp[i].card_id == (unsigned int)card_id)
            return temp[i].card_path;

    return 0;

}

int GetCardPriceModifier(unsigned int card_id)
{

    Card *temp = (Card *)cards->elements;
    for (size_t i = 0; i < cards->num_elements; i++)
        if (temp[i].card_id == (unsigned int)card_id)
            return temp[i].price_modifier;

    return 0;

}

float GetCardModifierLength(unsigned int card_id)
{

    Card *temp = (Card *)cards->elements;
    for (size_t i = 0; i < cards->num_elements; i++)
        if (temp[i].card_id == (unsigned int)card_id)
            return temp[i].modifier_length;

    return 0;

}

Vector *DBCards_GetPlayedModifiersCopy() 
{

    return Vector_GetCopy(played_cards);

}


int GetCardType(unsigned int card_id)
{

    if(GetCardPriceModifier(card_id) > 0)
        return 1;
    else
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

int GetPlayerCardId(int temp_card_id)
{

    PlayerCard *temp = (PlayerCard *)player_cards->elements;
    for (size_t i = 0; i < player_cards->num_elements; i++) {

        if (temp[i].card_id == (unsigned int)temp_card_id) {

            return temp[i].player_card_id;

        }
    }

    return 0;

}

void AddCardToPlayer(uint32_t player_id, uint32_t card_id)
{

    static uint32_t fake_unique_id = 1000000000;

    static char *query = "INSERT INTO Player_Cards (PlayerId CardId) VALUES (%d, %d);";
    Queue_PushMessage(card_queue, GetFormattedPointer(query, player_id, Game_GetSaveId(), card_id));

    PlayerCard temp;

    temp.player_card_id = fake_unique_id;
    temp.player_id      = player_id;
    temp.card_id        = card_id;

    fake_unique_id++;

    Vector_PushBack(player_cards, &temp);
}

void DBCards_ApplyCard(uint32_t card_id, uint32_t company_id)
{

    char *delete_query = "DELETE FROM Player_Cards WHERE PlayerCardId = (SELECT PC.PlayerCardId FROM PlayerCards PC WHERE PC.PlayerId = %d AND PC.CardId = %d LIMIT 1);";
    char *insert_query = "INSERT INTO Player_CardsPlayed (CardId, SaveId, CompanyId, PlayedTimed) VALUES (%d, %d, %d, %d)";

    PlayerCard *temp = player_cards->elements;

    for (size_t i = 0; i < player_cards->num_elements; i++) {
        Log("INLOOP");

        Queue_PushMessage(card_queue, GetFormattedPointer(delete_query, temp[i].player_id, temp[i].card_id));
        Queue_PushMessage(card_queue, GetFormattedPointer(insert_query, temp[i].card_id, Game_GetSaveId(), company_id, Game_GetGameTime()));

        if(temp[i].card_id == card_id)
        {
            LogF("Delete: %s", GetFormattedPointer(delete_query, temp[i].player_id, temp[i].card_id));
            LogF("dbcards i: %d", i);
            Vector_Remove(player_cards, i);
            break;
        }




        //LogF("temp[i].player_card_id == %d|player_card_id = %d", temp[i].player_card_id, player_card_id);
        //if (temp[i].player_card_id == player_card_id) {

        //    Queue_PushMessage(card_queue, GetFormattedPointer(delete_query, temp[i].player_id, temp[i].card_id));
        //    Queue_PushMessage(card_queue, GetFormattedPointer(insert_query, temp[i].card_id, Game_GetSaveId(), company_id, Game_GetGameTime()));

        //    LogF("Delete: %s", GetFormattedPointer(delete_query, temp[i].player_id, temp[i].card_id));
        //    LogF("dbcards i: %d", i);
        //    Vector_Remove(player_cards, i);
         //   break;

        //}

    }

}


int GetNumOfPlayerCards()
{

    assert(player_cards != NULL);
    return player_cards->num_elements;

}


PlayerCard *GetAllPlayerCards()
{

    assert(player_cards != NULL);
    return (PlayerCard *)player_cards->elements;

}

int GetNumOfPlayerPositiveCards()
{
    int amount = 0;

    PlayerCard *temp = (PlayerCard *)player_cards->elements;

    for(size_t i = 0; i < player_cards->num_elements; i++)  
        if(GetCardType(temp[i].card_id))
            amount++;

    return amount;

}

int GetNumOfPlayerNegativeCards()
{

    int amount = 0;
    PlayerCard *temp = (PlayerCard *)player_cards->elements;
    
    for(size_t i = 0; i < player_cards->num_elements; i++)  
        if(GetCardType(temp[i].card_id) == 0)
            amount++;

    return amount;
 
}

void SaveCards()
{

    Vector *vector = Queue_GetLockFreeVector(card_queue);

    sqlite3 *db;
    if (sqlite3_open_v2(DefaultConnection(), &db, SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK)
        return;

    ExecuteTransaction(db, vector);
    Vector_DeletePtrs(vector);

}

int DBCards_PlayedCardsCallback(void *played_card, int argc, char **argv, char **col_name)
{

    if (argc == 0)
        return 0;

    PlayedModifiers card;
    card.company_id      = atoi(argv[0]);
    card.played_time     = atol(argv[1]);
    card.price_modifier  = atof(argv[2]);
    card.modifier_length = atoi(argv[3]);

    Vector_PushBack(played_cards, &card);

    return 0;

}

void DBCards_InitVectors()
{

    if (player_cards != NULL) {

        Vector_Reset(player_cards);
        Vector_Reset(played_cards);

    } else {

        cards        = Vector_Create(sizeof(Card), 4);
        player_cards = Vector_Create(sizeof(PlayerCard), 4);
        played_cards = Vector_Create(sizeof(PlayedModifiers), 4); 

    }

}

void DBCards_Init()
{

    card_queue = Queue_Create();

    char *played_cards_query =
    "SELECT PCP.CompanyId, PCP.PlayedTime, SC.PriceModifier, SC.ModifierLength "
    "FROM Player_CardsPlayed PCP "
    "INNER JOIN System_Cards SC ON SC.CardId = PCP.CardId "
    "WHERE PCP.SaveId = %d";

    DBCards_InitVectors();
    ExecuteQueryF(&Card_Callback, NULL, "SELECT C.CardId, C.CardName, C.CardDesc, C.CardPath, C.PriceModifier, C.ModifierLength FROM System_Cards C");
    ExecuteQueryF(&PlayerCard_Callback, NULL, "SELECT C.PlayerCardId, C.PlayerId, C.CardId FROM Player_Cards C WHERE SaveId = %d", Game_GetSaveId());
    ExecuteQueryF(&DBCards_PlayedCardsCallback, NULL, played_cards_query, Game_GetSaveId());

}