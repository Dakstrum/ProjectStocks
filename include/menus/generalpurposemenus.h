#ifndef GENERALPURPOSEMENUS_H
#define GENERALPURPOSEMENUS_H

void InitializeGeneralPurposeMenu();
void GeneralPurposeMenus_InitSpeedSelectObject();

void ToggleGeneralPurposeMenus();

void GeneralPurposeMenus_InitStocksMenu_CB();
void GeneralPurposeMenus_InitNewsMenu_CB();
void GeneralPurposeMenus_InitAccountMenu_CB();
void GeneralPurposeMenus_InitCardsMenu_CB();

void GeneralPurposeMenus_SetSpeedOne_CB();
void GeneralPurposeMenus_SetSpeedTwo_CB();
void GeneralPurposeMenus_SetSpeedThree_CB();

void GeneralPurposeMenus_CleanSpeedButtons();
#endif