#include "drawobject.h"
#include "drawlayers.h"
#include "animations.h"
#include "log.h"
#include "controls.h"
#include "dbcard.h"
#include "menupersistence.h"

DrawObject *cardone_button   = NULL;
DrawObject *cardtwo_button   = NULL;
DrawObject *cardthree_button = NULL;
DrawObject *cardfour_button  = NULL;
DrawObject *cardfive_button  = NULL;

DrawObject *card_buttons[5];
PlayerCard *player_cards;


int number_of_cards = 0;
int played_card = -1;

void CardController_AnimateCardUp(int card_num);
int GetCenteredPoint(int i);

void CardController_CardOne_CB();
void CardController_CardTwo_CB();
void CardController_CardThree_CB();
void CardController_CardFour_CB();
void CardController_CardFive_CB();

void CardController_InitCard()
{
	card_buttons[0] = cardone_button;
	card_buttons[1] = cardtwo_button;
	card_buttons[2] = cardthree_button;
	card_buttons[3] = cardfour_button;
	card_buttons[4] = cardfive_button;

	void *card_CBs[5] = {CardController_CardOne_CB, CardController_CardTwo_CB, CardController_CardThree_CB, CardController_CardFour_CB, CardController_CardFive_CB};
	player_cards      = GetAllPlayerCards();
	number_of_cards   = GetNumOfPlayerCards();
	
	for(int i = 0; i < number_of_cards; i++)
	{

		card_buttons[i]         = CreateNewDrawObject();
		card_buttons[i]->type   = BUTTON;
        card_buttons[i]->x      = GetCenteredPoint(i);
        card_buttons[i]->y      = 1009;
        card_buttons[i]->width  = 165;
        card_buttons[i]->height = 230;
        card_buttons[i]->button.Callback = card_CBs[i];

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
			card_buttons[0]->x = card_buttons[0]->x + 136;
			return (centered_card_x - 82) + 45;

		case 2 :
			card_buttons[0]->x = card_buttons[0]->x + 90;
			card_buttons[1]->x = card_buttons[1]->x + 90;
			return (centered_card_x - (82 * 2)) + 45;

		case 3 :
			card_buttons[0]->x = card_buttons[0]->x + 90;
			card_buttons[1]->x = card_buttons[1]->x + 90;
			card_buttons[2]->x = card_buttons[2]->x + 90;
			return (centered_card_x - (82 * 3)) + 45;

		case 4 :
			card_buttons[0]->x = card_buttons[0]->x + 90;
			card_buttons[1]->x = card_buttons[1]->x + 90;
			card_buttons[2]->x = card_buttons[2]->x + 90;
			card_buttons[3]->x = card_buttons[3]->x + 90;
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

void CardController_AnimateCardToCenterOfGraph(int card_num)
{

	Animate_MoveDrawObject(card_buttons[card_num], 800, 300, 500);

}

void CardController_HoveringAnimationController(int card_num)
{

	if(card_buttons[card_num] == NULL)
		return;

	if(played_card == card_num)
	{
		Animate_MoveDrawObject(card_buttons[card_num], 800, 300, 500);

		if(card_buttons[card_num]->x == 800)
		{

			for(int i = 0; i < 5; i++)
				if(card_buttons[i])
					RemoveDrawObject(card_buttons[i]);

			CardController_InitCard();
			played_card = -1;

		}
	}
	
	else
	{

		if (HoveringOverBitmap(card_buttons[card_num]) == 1)
			CardController_AnimateCardUp(card_num);

		else if(HoveringOverBitmap(card_buttons[card_num]) == 0 && card_buttons[card_num]->y < 1009)
			CardController_AnimateCardDown(card_num);
	}



}

void CardController_CardAnimationCheck()
{
	
	for(int i = 0; i < 5; i++)
		CardController_HoveringAnimationController(i);

}

void CardController_CardOne_CB()
{

	DBCards_ApplyCard(player_cards[0].card_id, GetCompanyIdViewing());
	played_card = 0;

}

void CardController_CardTwo_CB()
{
	DBCards_ApplyCard(player_cards[1].card_id, GetCompanyIdViewing());
	played_card = 1;


}

void CardController_CardThree_CB()
{
	DBCards_ApplyCard(player_cards[2].card_id, GetCompanyIdViewing());
	played_card = 2;

}

void CardController_CardFour_CB()
{
	DBCards_ApplyCard(player_cards[3].card_id, GetCompanyIdViewing());
	played_card = 3;

}

void CardController_CardFive_CB()
{
	DBCards_ApplyCard(player_cards[4].card_id, GetCompanyIdViewing());
	played_card = 4;

}