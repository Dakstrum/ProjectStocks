#ifndef CARDCONTROLLER_H
#define CARDCONTROLLER_H

#include "manager.h"

void CardController_InitCard();
void CardController_Clean();

void CardController_HoveringAnimationController(int card_num);
void CardController_CardAnimationCheck();

void CardController_AnimateCardUp(int card_num);


#endif