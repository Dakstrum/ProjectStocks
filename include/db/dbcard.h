#ifndef DBCARDS_H
#define DBCARDS_H

#include <time.h>
#include <stdint.h>

#include "vector.h"

typedef struct Card {

    uint32_t card_id;
    char card_name[32];
    char card_desc[255];
    char card_path[255];
    float price_modifier;
    uint32_t modifier_length;

} Card;

typedef struct PlayerCard {

	uint32_t player_card_id;
	uint32_t player_id;
	uint32_t card_id;

} PlayerCard;

typedef struct PlayedModifiers {

	uint32_t company_id;
	time_t played_time;
	float price_modifier;
	uint32_t modifier_length;

} PlayedModifiers;


int GetCardId(char* card_title);
char* GetCardPath(unsigned int card_id);
int GetCardPriceModifier(unsigned int card_id);
float GetCardModifierLength(unsigned int card_id);
char* GetCardDescription(unsigned int card_id);
int GetNumOfPlayerPositiveCards();
int GetNumOfPlayerNegativeCards();
PlayerCard *GetAllPlayerCards();
char* GetCardTitle(unsigned int card_id);
int GetCardType(unsigned int card_id);
int GetNumOfPlayerCards();
int GetPlayerCardId(int temp_card_id);
void SaveCards();

Vector *dbcard_get_player_cards(uint32_t player_id);

void dbcard_add_card_to_player(uint32_t player_id, uint32_t card_id);
void dbcard_apply_card(uint32_t card_id, uint32_t company_id);
void dbcard_init();

Vector *dbcard_get_played_modifiers_copy();
Vector *dbcard_get_all_cards();


#endif