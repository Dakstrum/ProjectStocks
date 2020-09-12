#ifndef DBCARDS_H
#define DBCARDS_H

#include "vector.h"

typedef struct Card {

    unsigned int card_id;
    char card_name[32];
    char card_path[255];
    float price_modifier;
    unsigned int modifier_length;

} Card;


void InitializeCards();
unsigned int GetCardId(char* card_title);
char* GetCardPath(char* card_title);
int GetCardPriceModifier(char* card_title);
float GetCardModifierLength(char* card_title);

#endif