#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "log.h"
#include "drawlayers.h"
#include "shared.h"

#include "mainmenu.h"
#include "stocksmenu.h"
#include "newsmenu.h"
#include "accountmenu.h"
#include "generalpurposemenus.h"
#include "savemenus.h"
#include "cardsmenu.h"
#include "vector.h"
#include "optionsmenu.h"
#include "pausemenus.h"


typedef struct ButtonCallbacks 
{

    char *name;
    void (*Callback)();

} ButtonCallbacks;

static Vector *button_callbacks = NULL;

void StubCallBack() 
{

    Log("STUB: Button may have not been set correctly StubCallback");

}

void *GetButtonCallback(char *button_name) 
{

    ButtonCallbacks *callbacks = (ButtonCallbacks *)button_callbacks->elements;

    for (size_t i = 0; i < button_callbacks->num_elements; i++) 
        if (strcmp(button_name, callbacks[i].name) == 0) 
            return callbacks[i].Callback;

    return &StubCallBack;

}

void AddButton(char *button_name, void (*Callback)())
{

    ButtonCallbacks button_callback = {button_name, Callback};
    Vector_PushBack(button_callbacks, &button_callback);

}

void MainMenu_InitButtons() 
{

    AddButton("MainMenuStartButtonObject",   &MainMenu_Start_CB);
    AddButton("MainMenuOptionsButtonObject", &MainMenu_Options_CB);
    AddButton("MainMenuExitButtonObject",    &MainMenu_Exit_CB);
    AddButton("MainMenuGithubButtonObject",  &MainMenu_GitHub_CB);
    AddButton("MainMenuTwitterButtonObject", &MainMenu_Twitter_CB);
    AddButton("MainMenuYoutubeButtonObject", &MainMenu_Youtube_CB);
    AddButton("MainMenuWebsiteButtonObject", &MainMenu_Website_CB);

}

void StocksMenu_InitButtons()
{

    AddButton("StocksMenuStocksTabButtonObject",  &GeneralPurposeMenus_InitStocksMenu_CB);
    AddButton("StocksMenuNewsTabButtonObject",    &GeneralPurposeMenus_InitNewsMenu_CB);
    AddButton("StocksMenuAccountTabButtonObject", &GeneralPurposeMenus_InitAccountMenu_CB);
    AddButton("StocksMenuCardsTabButtonObject",   &GeneralPurposeMenus_InitCardsMenu_CB);

    AddButton("StocksMenuSellButtonObject",       &StocksMenu_SellMenu_BCB);
    AddButton("StocksMenuBuyButtonObject",        &StocksMenu_BuyMenu_BCB);

    AddButton("StocksMenuDButtonObject",          &OneD_BCB);
    AddButton("StocksMenuWButtonObject",          &OneW_BCB);
    AddButton("StocksMenuMButtonObject",          &OneM_BCB);
    AddButton("StocksMenuYButtonObject",          &OneY_BCB);
    AddButton("StocksMenuAllButtonObject",        &All_BCB);

    AddButton("BuyMenuExitButtonObject",          &StocksMenu_BuyMenu_BCB);
    AddButton("BuyMenuBuyButtonObject",           &StocksMenu_BuyMenu_Buy_BCB);

    AddButton("SellMenuExitButtonObject",         &StocksMenu_SellMenu_BCB);
    AddButton("SellMenuSellButtonObject",         &StocksMenu_BuyMenu_Sell_BCB);

    AddButton("StocksMenuSpeed1ButtonObject",   &GeneralPurposeMenus_SetSpeedOne_CB);
    AddButton("StocksMenuSpeed2ButtonObject",   &GeneralPurposeMenus_SetSpeedTwo_CB);
    AddButton("StocksMenuSpeed3ButtonObject",   &GeneralPurposeMenus_SetSpeedThree_CB);
    
}

void InitializeSaveMenuButtons()            
{

    AddButton("LoadSaveMenuNewButtonObject",        &LoadSaveMenu_NewSave_CB);
    AddButton("LoadSaveMenuLoadButtonObject",       &LoadSaveMenu_LoadSave_CB);
    AddButton("LoadSaveMenuBackButtonObject",       &LoadSaveMenu_Back_CB);
    AddButton("LoadSaveMenuDeleteSaveButtonObject", &LoadSaveMenu_DeleteSave_CB);

    AddButton("NewSaveMenuBackButtonObject",        &NewSaveMenu_Back_CB);
    AddButton("NewSaveMenuCreateButtonObject",      &NewSaveMenu_CreateSave_CB);

}

void InitializeAccountButtons()
{

    AddButton("AccountMenuStocksTabButtonObject",  &GeneralPurposeMenus_InitStocksMenu_CB);
    AddButton("AccountMenuNewsTabButtonObject",    &GeneralPurposeMenus_InitNewsMenu_CB);
    AddButton("AccountMenuAccountTabButtonObject", &GeneralPurposeMenus_InitAccountMenu_CB);
    AddButton("AccountMenuCardsTabButtonObject",   &GeneralPurposeMenus_InitCardsMenu_CB);

    AddButton("AccountMenuDownButtonObject", &AccountMenu_Down_CB);
    AddButton("AccountMenuUpButtonObject",   &AccountMenu_Up_CB);

    AddButton("AccountMenuSpeed1ButtonObject",   &GeneralPurposeMenus_SetSpeedOne_CB);
    AddButton("AccountMenuSpeed2ButtonObject",   &GeneralPurposeMenus_SetSpeedTwo_CB);
    AddButton("AccountMenuSpeed3ButtonObject",   &GeneralPurposeMenus_SetSpeedThree_CB);

}

void InitializeNewsButtons()
{

    AddButton("NewsMenuStocksTabButtonObject",  &GeneralPurposeMenus_InitStocksMenu_CB);
    AddButton("NewsMenuNewsTabButtonObject",    &GeneralPurposeMenus_InitNewsMenu_CB);
    AddButton("NewsMenuAccountTabButtonObject", &GeneralPurposeMenus_InitAccountMenu_CB);
    AddButton("NewsMenuCardsTabButtonObject",   &GeneralPurposeMenus_InitCardsMenu_CB);

    AddButton("NewsMenuSpeed1ButtonObject",   &GeneralPurposeMenus_SetSpeedOne_CB);
    AddButton("NewsMenuSpeed2ButtonObject",   &GeneralPurposeMenus_SetSpeedTwo_CB);
    AddButton("NewsMenuSpeed3ButtonObject",   &GeneralPurposeMenus_SetSpeedThree_CB);

    AddButton("NewsMenuscrollbox_backButtonObject", &NewsMenu_ScrollboxBack_CB);

}

void InitializeGeneralPurposeButtons()
{

    AddButton("StocksButton", &GeneralPurposeMenus_InitStocksMenu_CB);
    AddButton("NewsButton", &GeneralPurposeMenus_InitNewsMenu_CB);
    AddButton("AccountButton", &GeneralPurposeMenus_InitAccountMenu_CB);
    AddButton("CardsButton", &GeneralPurposeMenus_InitCardsMenu_CB);

    AddButton("SpeedOneButton", &GeneralPurposeMenus_SetSpeedOne_CB);
    AddButton("SpeedTwoButton", &GeneralPurposeMenus_SetSpeedTwo_CB);
    AddButton("SpeedThreeButton", &GeneralPurposeMenus_SetSpeedThree_CB);

}

void CardsMenu_InitButtons()
{

    AddButton("CardsMenuStocksTabButtonObject",  &GeneralPurposeMenus_InitStocksMenu_CB);
    AddButton("CardsMenuNewsTabButtonObject",    &GeneralPurposeMenus_InitNewsMenu_CB);
    AddButton("CardsMenuAccountTabButtonObject", &GeneralPurposeMenus_InitAccountMenu_CB);
    AddButton("CardsMenuCardsTabButtonObject",   &GeneralPurposeMenus_InitCardsMenu_CB);

    AddButton("CardsMenuApplyButtonObject", &CardsMenu_PickCompanyMenu_CB);


    AddButton("ApplyCardMenuExitButtonObject", &CardsMenu_PickCompanyMenu_CB);

    AddButton("CardsMenuSpeed1ButtonObject",   &GeneralPurposeMenus_SetSpeedOne_CB);
    AddButton("CardsMenuSpeed2ButtonObject",   &GeneralPurposeMenus_SetSpeedTwo_CB);
    AddButton("CardsMenuSpeed3ButtonObject",   &GeneralPurposeMenus_SetSpeedThree_CB);

}


void InitializePauseMenusButtons()
{

    AddButton("PauseMenuMainMenuButtonObject", &PauseMenu_MainMenu_CB);
    AddButton("PauseMenuOptionsButtonObject",  &PauseMenu_Options_CB);
    AddButton("PauseMenuSaveButtonObject",     &PauseMenu_Save_CB);
    AddButton("PauseMenuExitButtonObject",     &PauseMenu_Exit_CB);
    AddButton("PauseMenuResumeButtonObject",   &PauseMenu_Toggle);

}

void InitializeOptionsButtons()
{

    AddButton("OptionsMenuOptionsBackButtonObject", &OptionsMenu_Exit_CB);
    AddButton("OptionsMenuDisplayButtonObject",     &OptionsMenu_DisplayMenu_Toggle);
    AddButton("OptionsMenuAudioButtonObject",       &OptionsMenu_AudioMenu_Toggle);

    AddButton("DisplayMenuOptionsBackButtonObject", &OptionsMenu_DisplayMenu_Exit_CB);
    AddButton("DisplayMenuAudioButtonObject",       &OptionsMenu_AudioMenu_Toggle);
    AddButton("DisplayMenuFullScreenButtonObject",  &OptionsMenu_DisplayMenu_FullScreen_CB);
    AddButton("DisplayMenuResolutionButtonObject",  &OptionsMenu_DisplayMenu_InitResolutionScrollbox);

    AddButton("AudioMenuOptionsBackButtonObject",   &OptionsMenu_AudioMenu_Exit_CB);
    AddButton("AudioMenuDisplayButtonObject",       &OptionsMenu_DisplayMenu_Toggle);

}




void InitializeButtons() 
{

    button_callbacks = Vector_Create(sizeof(ButtonCallbacks), 16);

    AddButton("STUB", &StubCallBack);

    MainMenu_InitButtons();
    StocksMenu_InitButtons();
    InitializeAccountButtons();
    InitializeGeneralPurposeButtons();
    InitializePauseMenusButtons();
    InitializeOptionsButtons();
    InitializeSaveMenuButtons();
    CardsMenu_InitButtons();
    InitializeNewsButtons();

}