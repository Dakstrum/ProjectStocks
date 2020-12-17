#ifndef CARDMANAGER_H
#define CARDMANAGER_H

#include "manager.h"

void CardManager_InitCard();

void CardManager_HoveringAnimationController(int card_num);
void CardManager_CardAnimationCheck();

void CardManager_AnimateCardUp(int card_num);


#endif