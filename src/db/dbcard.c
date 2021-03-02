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

    return NULL;

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

Vector *dbcard_get_all_cards()
{

    assert(cards != NULL);
    return cards;

}

Vector *dbcard_get_played_cards()
{

    return played_cards;

}

Vector *dbcard_get_played_cards_copy() 
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

void dbcard_add_card_to_player(uint32_t player_id, uint32_t card_id)
{

    static uint32_t fake_unique_id = 1000000000;

    static char *query = "INSERT INTO Player_Cards (PlayerId, CardId) VALUES (%d, %d);";
    Queue_PushMessage(card_queue, GetFormattedPointer(query, player_id, card_id));

    PlayerCard temp;

    temp.player_card_id = fake_unique_id;
    temp.player_id      = player_id;
    temp.card_id        = card_id;

    fake_unique_id++;

    Vector_PushBack(player_cards, &temp);
}

void dbcard_apply_card(uint32_t player_id, uint32_t card_id, uint32_t company_id)
{

    char *update_query = "UPDATE Player_Cards SET Played = 1 WHERE PlayerCardId = (SELECT PC.PlayerCardId FROM Player_Cards PC WHERE PC.PlayerId = %d AND PC.CardId = %d AND PC.Played=0 LIMIT 1);";
    char *insert_query = "INSERT INTO Player_CardsPlayed (CardId, PlayerId, CompanyId, PlayedTime) VALUES (%d, %d, %d, %d)";

    PlayerCard *temp = player_cards->elements;

    for (size_t i = 0; i < player_cards->num_elements; i++) {

        if (player_id != temp[i].player_id)
            continue;

        if (card_id != temp[i].card_id)
            continue;

        Queue_PushMessage(card_queue, GetFormattedPointer(update_query, player_id, card_id));
        Queue_PushMessage(card_queue, GetFormattedPointer(insert_query, card_id, player_id, company_id, Game_GetGameTime()));

        Vector_Remove(player_cards, i);
        break;
        
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

Vector *dbcard_get_player_cards(uint32_t player_id)
{

    Vector *specific_player_cards = Vector_Create(sizeof(PlayerCard), 5);

    PlayerCard *player_cards_temp = player_cards->elements;
    for (size_t i = 0; i < player_cards->num_elements;i++)
        if (player_cards_temp[i].player_id == player_id)
            Vector_PushBack(specific_player_cards, &player_cards_temp[i]);

    return specific_player_cards;

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

void dbcard_save_cards()
{

    Vector *vector = Queue_GetLockFreeVector(card_queue);

    sqlite3 *db;
    if (sqlite3_open_v2(DefaultConnection(), &db, SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK)
        return;

    ExecuteTransaction(db, vector);
    Vector_DeletePtrs(vector);

}

int dbcard_played_cards_callback(void *played_card, int argc, char **argv, char **col_name)
{

    if (argc == 0)
        return 0;

    PlayedCard card;
    card.company_id      = atoi(argv[0]);
    card.played_time     = atol(argv[1]);
    card.price_modifier  = atof(argv[2]);
    card.modifier_length = atoi(argv[3]);
    card.player_id       = atoi(argv[4]);
    card.card_id         = atoi(argv[5]);

    Vector_PushBack(played_cards, &card);

    return 0;

}

void dbcard_init_vectors()
{

    if (player_cards != NULL) {

        Vector_Reset(player_cards);
        Vector_Reset(played_cards);

    } else {

        cards        = Vector_Create(sizeof(Card), 4);
        player_cards = Vector_Create(sizeof(PlayerCard), 4);
        played_cards = Vector_Create(sizeof(PlayedCard), 4); 

    }

}

void dbcard_init()
{

    if (card_queue != NULL)
        Queue_Delete(card_queue);

    card_queue = Queue_Create();

    char *played_cards_query =
    "SELECT PCP.CompanyId, PCP.PlayedTime, SC.PriceModifier, SC.ModifierLength, PCP.PlayerId, PCP.CardId "
    "FROM Player_CardsPlayed PCP "
    "INNER JOIN Game_Players GP ON GP.PlayerId = PCP.PlayerId "
    "INNER JOIN System_Cards SC ON SC.CardId = PCP.CardId "
    "WHERE GP.SaveId = %d";

    char *player_cards_query =
    "SELECT PC.PlayerCardId, PC.PlayerId, PC.CardId " 
    "FROM Player_Cards PC "
    "INNER JOIN Game_Players GP ON GP.PlayerId = PC.PlayerId "
    "WHERE "
    "    GP.SaveId = %d AND PC.Played = 0";

    dbcard_init_vectors();
    ExecuteQueryF(&Card_Callback, NULL, "SELECT C.CardId, C.CardName, C.CardDesc, C.CardPath, C.PriceModifier, C.ModifierLength FROM System_Cards C");
    ExecuteQueryF(&PlayerCard_Callback, NULL, player_cards_query, Game_GetSaveId());
    ExecuteQueryF(&dbcard_played_cards_callback, NULL, played_cards_query, Game_GetSaveId());

}