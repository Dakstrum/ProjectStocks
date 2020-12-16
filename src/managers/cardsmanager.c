#include "drawobject.h"
#include "drawlayers.h"
#include "animations.h"
#include "log.h"
#include "controls.h"
#include "dbcard.h"

static DrawObject *cardone_button   = NULL;
static DrawObject *cardtwo_button   = NULL;
static DrawObject *cardthree_button = NULL;
static DrawObject *cardfour_button  = NULL;
static DrawObject *cardfive_button  = NULL;

DrawObject *card_buttons[5];

void CardManager_AnimateCardUp(int card_num);

void CardManager_InitCard()
{
	card_buttons[1] = cardone_button;
	card_buttons[2] = cardtwo_button;
	card_buttons[3] = cardthree_button;
	card_buttons[4] = cardfour_button;
	card_buttons[5] = cardfive_button;

	static int card_buttons_x[5] = {414, 611, 814, 1011, 1215};

	PlayerCard *player_cards  = GetAllPlayerCards();

	for(int i = 1; i < GetNumOfPlayerCards() + 1; i++)
	{
		
		card_buttons[i]    = CreateNewDrawObject();

		card_buttons[i]->type   = BUTTON;
        card_buttons[i]->x      = card_buttons_x[i - 1];
        card_buttons[i]->y      = 1009;
        card_buttons[i]->width  = 165;
        card_buttons[i]->height = 230;

		card_buttons[i]->asset_path = GetCardPath(player_cards[i - 1].card_id);

		AddObjectToDrawLayer(card_buttons[i]);
	
	}

}

void CardManager_AnimateCardUp(int card_num)
{

	Animate_MoveDrawObject(card_buttons[card_num], card_buttons[card_num]->x, 800, 500);

}

void CardManager_AnimateCardDown(int card_num)
{

	Animate_MoveDrawObject(card_buttons[card_num], card_buttons[card_num]->x, 1009, 500);

}

void CardManager_HoveringAnimationController(int card_num)
{

	if (HoveringOverBitmap(card_buttons[card_num]) == 1)
		CardManager_AnimateCardUp(card_num);

	else if(HoveringOverBitmap(card_buttons[card_num]) == 0 && card_buttons[card_num]->y < 1009)
		CardManager_AnimateCardDown(card_num);
	
	else
		return;

}


