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

static int number_of_cards = 0;

void CardController_AnimateCardUp(int card_num);
int GetCenteredPoint(int i);

void CardController_InitCard()
{
	card_buttons[0] = cardone_button;
	card_buttons[1] = cardtwo_button;
	card_buttons[2] = cardthree_button;
	card_buttons[3] = cardfour_button;
	card_buttons[4] = cardfive_button;

	PlayerCard *player_cards  = GetAllPlayerCards();
	number_of_cards           = GetNumOfPlayerCards();

	for(int i = 0; i < number_of_cards; i++)
	{
		
		card_buttons[i]    = CreateNewDrawObject();
		LogF(" i = %d||||point: %d", i, GetCenteredPoint(i));
		card_buttons[i]->type   = BUTTON;
        card_buttons[i]->x      = GetCenteredPoint(i);
        card_buttons[i]->y      = 1009;
        card_buttons[i]->width  = 165;
        card_buttons[i]->height = 230;

		card_buttons[i]->asset_path = GetCardPath(player_cards[i].card_id);

		AddObjectToDrawLayer(card_buttons[i]);
	
	}

}

int GetCenteredPoint(int i)
{
	int centered_card_x = 770;

	switch(i) {

		case 0 :
			return centered_card_x;

		case 1 :
			card_buttons[0]->x = card_buttons[0]->x + 68;
			return (centered_card_x - 82) + 45;

		case 2 :
			card_buttons[0]->x = card_buttons[0]->x + 45;
			card_buttons[1]->x = card_buttons[1]->x + 45;
			return (centered_card_x - (82 * 2)) + 45;

		case 3 :
			card_buttons[0]->x = card_buttons[0]->x + 45;
			card_buttons[1]->x = card_buttons[1]->x + 45;
			card_buttons[2]->x = card_buttons[2]->x + 45;
			return (centered_card_x - (82 * 3)) + 45;

		case 4 :
			card_buttons[0]->x = card_buttons[0]->x + 45;
			card_buttons[1]->x = card_buttons[1]->x + 45;
			card_buttons[2]->x = card_buttons[2]->x + 45;
			card_buttons[3]->x = card_buttons[3]->x + 45;
			return (centered_card_x - (82 * 4)) + 45;

	}
	
	return 0;

}

void CardController_AnimateCardUp(int card_num)
{

	Animate_MoveDrawObject(card_buttons[card_num], card_buttons[card_num]->x, 850, 500);

}

void CardController_AnimateCardDown(int card_num)
{

	Animate_MoveDrawObject(card_buttons[card_num], card_buttons[card_num]->x, 1009, 500);

}

void CardController_HoveringAnimationController(int card_num)
{
	if(card_buttons[card_num] == NULL)
		return;

	if (HoveringOverBitmap(card_buttons[card_num]) == 1)
		CardController_AnimateCardUp(card_num);

	else if(HoveringOverBitmap(card_buttons[card_num]) == 0 && card_buttons[card_num]->y < 1009)
		CardController_AnimateCardDown(card_num);
	
	else
		return;

}

void CardController_CardAnimationCheck()
{
	for(int i = 0; i <= number_of_cards; i++)
	{

		CardController_HoveringAnimationController(i);

	}

}