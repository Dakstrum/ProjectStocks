#ifndef DBCARDS_H
#define DBCARDS_H

#include "vector.h"

typedef struct Card {

    unsigned int card_id;
    char card_name[32];
    char card_desc[255];
    char card_path[255];
    float price_modifier;
    unsigned int modifier_length;

} Card;

typedef struct PlayerCard {

	unsigned int player_card_id;
	unsigned int player_id;
	unsigned int card_id;

} PlayerCard;


void InitializeCards();
int GetCardId(char* card_title);
char* GetCardPath(unsigned int card_id);
int GetCardPriceModifier(unsigned int card_id);
float GetCardModifierLength(unsigned int card_id);
char* GetCardDescription(unsigned int card_id);
void AddCardToPlayer(int card_id);
int GetNumOfPlayerPositiveCards();
int GetNumOfPlayerNegativeCards();
void InitializePlayerCards();
PlayerCard *GetAllPlayerCards();
char* GetCardTitle(unsigned int card_id);
int GetCardType(unsigned int card_id);
int GetNumOfPlayerCards();
void RemoveCardFromPlayer(unsigned int player_card_id);
int GetPlayerCardId(int temp_card_id);
void InitializeCardInformation();
void SaveCards();


#endif