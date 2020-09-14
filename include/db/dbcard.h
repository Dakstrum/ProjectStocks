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

	int player_card_id;
	int player_id;
	int card_id;

} PlayerCard;


void InitializeCards();
unsigned int GetCardId(char* card_title);
char* GetCardPath(char* card_title);
int GetCardPriceModifier(char* card_title);
float GetCardModifierLength(char* card_title);
char* GetCardDescription(char* card_title);
void AddCardToPlayer(int card_id);
int GetNumOfPlayerPositiveCards();
int GetNumOfPlayerNegativeCards();
void InitializePlayerCards();


#endif