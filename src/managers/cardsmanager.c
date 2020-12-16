#include "drawobject.h"
#include "drawlayers.h"
#include "animations.h"
#include "log.h"
#include "controls.h"

static DrawObject *cardone_bitmap   = NULL;
static DrawObject *cardtwo_bitmap   = NULL;
static DrawObject *cardthree_bitmap = NULL;
static DrawObject *cardfour_bitmap  = NULL;
static DrawObject *cardfive_bitmap  = NULL;

DrawObject *card_bitmaps[5];

void CardManager_AnimateCardUp(int card_num);

void CardManager_InitCard()
{
	card_bitmaps[1] = cardone_bitmap;
	card_bitmaps[2] = cardtwo_bitmap;
	card_bitmaps[3] = cardthree_bitmap;
	card_bitmaps[4] = cardfour_bitmap;
	card_bitmaps[5] = cardfive_bitmap;

	static int card_bitmaps_x[5] = {414, 611, 814, 1011, 1215};

	for(int i = 0; i < 5; i++)
	{
		
		card_bitmaps[i]    = CreateNewDrawObject();

		card_bitmaps[i]->type   = MENU;
        card_bitmaps[i]->x      = card_bitmaps_x[i];
        card_bitmaps[i]->y      = 1009;
        card_bitmaps[i]->width  = 165;
        card_bitmaps[i]->height = 230;

		card_bitmaps[i]->asset_path = "assets/images/cards/deadlyproduct.png";

		AddObjectToDrawLayer(card_bitmaps[i]);
	
	}

}

void CardManager_AnimateCardUp(int card_num)
{

	Animate_MoveDrawObject(card_bitmaps[card_num], card_bitmaps[card_num]->x, 800, 500);

}

void CardManager_AnimateCardDown(int card_num)
{

	Animate_MoveDrawObject(card_bitmaps[card_num], card_bitmaps[card_num]->x, 1009, 500);

}

void CardManager_HoveringAnimationController(int card_num)
{

	if (HoveringOverBitmap(card_bitmaps[card_num]) == 1)
		CardManager_AnimateCardUp(card_num);

	else if(HoveringOverBitmap(card_bitmaps[card_num]) == 0 && card_bitmaps[card_num]->y < 1009)
		CardManager_AnimateCardDown(card_num);
	
	else
		return;

}


