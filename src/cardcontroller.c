#include "drawobject.h"
#include "drawlayers.h"
#include "animations.h"
#include "log.h"
#include "controls.h"
#include "dbcard.h"
#include "account.h"
#include "vector.h"
#include "menupersistence.h"

static DrawObject *card_buttons[5];
static Vector *player_cards = NULL;

static int played_card    = -1;
static int card_animating = 0;

void CardController_AnimateCardUp(int card_num);
int GetCenteredPoint(int i);

void CardController_CardOne_CB();
void CardController_CardTwo_CB();
void CardController_CardThree_CB();
void CardController_CardFour_CB();
void CardController_CardFive_CB();

static const void *card_CBs[5] = 
{
	CardController_CardOne_CB, 
	CardController_CardTwo_CB, 
	CardController_CardThree_CB, 
	CardController_CardFour_CB, 
	CardController_CardFive_CB
};

void CardController_Reset()
{
	for (size_t i = 0; i < 5; i++) {

		if (card_buttons[i]) {

			RemoveDrawObject(card_buttons[i]);
			card_buttons[i] = NULL;

		}	
	}

	if (player_cards != NULL)
		Vector_Delete(player_cards);

}

void CardController_Set()
{

	player_cards = dbcard_get_player_cards(Account_GetPlayerId());
	LogF("PlayerId = %u", Account_GetPlayerId());
	PlayerCard *player_cards_temp = player_cards->elements;
	assert(player_cards->num_elements < 6);
	for (size_t i = 0; i < player_cards->num_elements; i++) {

		card_buttons[i]         = CreateNewDrawObject();
		card_buttons[i]->type   = BUTTON;
        card_buttons[i]->x      = GetCenteredPoint(i);
        card_buttons[i]->y      = 1009;
        card_buttons[i]->width  = 165;
        card_buttons[i]->height = 230;
        card_buttons[i]->button.Callback = card_CBs[i];

		card_buttons[i]->asset_path = GetCardPath(player_cards_temp[i].card_id);

		AddObjectToDrawLayer(card_buttons[i]);
	
	}
	
}

void CardController_InitCard()
{

	CardController_Reset();
	CardController_Set();
	
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

	if (card_buttons[card_num] == NULL)
		return;

	if (played_card == card_num) {

		card_animating = 1;
		Animate_MoveDrawObject(card_buttons[card_num], 800, 300, 500);

		if (card_buttons[card_num]->x == 800) {

			CardController_InitCard();
			played_card    = -1;
			card_animating = 0;

		}

	} else {
		
		if (HoveringOverBitmap(card_buttons[card_num]) == 1)
			CardController_AnimateCardUp(card_num);

		else if (HoveringOverBitmap(card_buttons[card_num]) == 0 && card_buttons[card_num]->y < 1009)
			CardController_AnimateCardDown(card_num);

	}

}

void CardController_CardAnimationCheck()
{

	for (int i = 0; i < 5; i++)
		CardController_HoveringAnimationController(i);

}

void cardcontroller_card_cb(uint32_t idx)
{

	if(card_animating)
		return;

	PlayerCard *player_cards_temp = player_cards->elements;
	dbcard_apply_card(player_cards_temp[idx].card_id, GetCompanyIdViewing());
	played_card = idx;

}

void CardController_CardOne_CB()
{

	cardcontroller_card_cb(0);

}

void CardController_CardTwo_CB()
{

	cardcontroller_card_cb(1);

}

void CardController_CardThree_CB()
{

	cardcontroller_card_cb(2);

}

void CardController_CardFour_CB()
{

	cardcontroller_card_cb(3);

}

void CardController_CardFive_CB()
{

	cardcontroller_card_cb(4);

}