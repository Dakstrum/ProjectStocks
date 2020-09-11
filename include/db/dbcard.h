#ifndef DBCARDS_H
#define DBCARDS_H

typedef struct Card {

    unsigned int card_id;
    char card_name[32];
    char card_path[255];
    float price_modifier;
    unsigned int modifier_length;

} Card;


void InitializeCards();
unsigned int GetCardId(char* card_title);

#endif