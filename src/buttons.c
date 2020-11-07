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

void InitializeMainMenuButtons() 
{

    AddButton("MainMenuStartButtonObject",   &Start_BCB);
    AddButton("MainMenuOptionsButtonObject", &MainMenuOptions_BCB);
    AddButton("MainMenuExitButtonObject",    &MainMenuExit_BCB);
    AddButton("MainMenuGithubButtonObject",  &GitHub_BCB);
    AddButton("MainMenuTwitterButtonObject", &Twitter_BCB);
    AddButton("MainMenuYoutubeButtonObject", &Youtube_BCB);
    AddButton("MainMenuWebsiteButtonObject", &Website_BCB);

}

void InitializeStocksMenuButtons()
{

    AddButton("StocksMenuStocksTabButtonObject",  &Stocks_BCB);
    AddButton("StocksMenuNewsTabButtonObject",    &News_BCB);
    AddButton("StocksMenuAccountTabButtonObject", &Account_BCB);
    AddButton("StocksMenuCardsTabButtonObject",   &Cards_BCB);

    AddButton("StocksMenuSellButtonObject",       &SellMenu_BCB);
    AddButton("StocksMenuBuyButtonObject",        &BuyMenu_BCB);

    AddButton("StocksMenuDButtonObject",          &OneD_BCB);
    AddButton("StocksMenuWButtonObject",          &OneW_BCB);
    AddButton("StocksMenuMButtonObject",          &OneM_BCB);
    AddButton("StocksMenuYButtonObject",          &OneY_BCB);
    AddButton("StocksMenuAllButtonObject",        &All_BCB);

    AddButton("BuyMenuExitButtonObject",          &BuyMenu_BCB);
    AddButton("BuyMenuBuyButtonObject",           &Buy_BCB);

    AddButton("SellMenuExitButtonObject",         &SellMenu_BCB);
    AddButton("SellMenuSellButtonObject",         &Sell_BCB);

    AddButton("StocksMenuSpeed1ButtonObject",   &SpeedOne_BCB);
    AddButton("StocksMenuSpeed2ButtonObject",   &SpeedTwo_BCB);
    AddButton("StocksMenuSpeed3ButtonObject",   &SpeedThree_BCB);
    
}

void InitializeSaveMenuButtons()            
{

    AddButton("LoadSaveMenuNewButtonObject",        &NewSaveMenu_BCB);
    AddButton("LoadSaveMenuLoadButtonObject",       &LoadSaveMenuLoad_BCB);
    AddButton("LoadSaveMenuBackButtonObject",       &LoadSaveMenuBack_BCB);
    AddButton("LoadSaveMenuDeleteSaveButtonObject", &DeleteSave_BCB);

    AddButton("NewSaveMenuBackButtonObject",        &NewSaveMenuBack_BCB);
    AddButton("NewSaveMenuCreateButtonObject",      &CreateSave_BCB);

}

void InitializeAccountButtons()
{

    AddButton("AccountMenuStocksTabButtonObject",  &Stocks_BCB);
    AddButton("AccountMenuNewsTabButtonObject",    &News_BCB);
    AddButton("AccountMenuAccountTabButtonObject", &Account_BCB);
    AddButton("AccountMenuCardsTabButtonObject",   &Cards_BCB);

    AddButton("AccountMenuDownButtonObject", &AccountDown_BCB);
    AddButton("AccountMenuUpButtonObject",   &AccountUp_BCB);

    AddButton("AccountMenuSpeed1ButtonObject",   &SpeedOne_BCB);
    AddButton("AccountMenuSpeed2ButtonObject",   &SpeedTwo_BCB);
    AddButton("AccountMenuSpeed3ButtonObject",   &SpeedThree_BCB);

}

void InitializeNewsButtons()
{

    AddButton("NewsMenuStocksTabButtonObject",  &Stocks_BCB);
    AddButton("NewsMenuNewsTabButtonObject",    &News_BCB);
    AddButton("NewsMenuAccountTabButtonObject", &Account_BCB);
    AddButton("NewsMenuCardsTabButtonObject",   &Cards_BCB);

    AddButton("NewsMenuSpeed1ButtonObject",   &SpeedOne_BCB);
    AddButton("NewsMenuSpeed2ButtonObject",   &SpeedTwo_BCB);
    AddButton("NewsMenuSpeed3ButtonObject",   &SpeedThree_BCB);

    AddButton("NewsMenuscrollbox_backButtonObject", &ScrollboxBack_BCB);

}

void InitializeGeneralPurposeButtons()
{

    AddButton("StocksButton", &Stocks_BCB);
    AddButton("NewsButton", &News_BCB);
    AddButton("AccountButton", &Account_BCB);
    AddButton("CardsButton", &Cards_BCB);

    AddButton("PauseSpeedButton", &PauseSpeed_BCB);
    AddButton("SpeedOneButton", &SpeedOne_BCB);
    AddButton("SpeedTwoButton", &SpeedTwo_BCB);
    AddButton("SpeedThreeButton", &SpeedThree_BCB);

}

void InitializeCardsMenuButtons()
{

    AddButton("CardsMenuStocksTabButtonObject",  &Stocks_BCB);
    AddButton("CardsMenuNewsTabButtonObject",    &News_BCB);
    AddButton("CardsMenuAccountTabButtonObject", &Account_BCB);
    AddButton("CardsMenuCardsTabButtonObject",   &Cards_BCB);

    AddButton("CardsMenuApplyButtonObject", &ApplyMenu_BCB);


    AddButton("ApplyCardMenuExitButtonObject", &ApplyMenu_BCB);

    AddButton("CardsMenuSpeed1ButtonObject",   &SpeedOne_BCB);
    AddButton("CardsMenuSpeed2ButtonObject",   &SpeedTwo_BCB);
    AddButton("CardsMenuSpeed3ButtonObject",   &SpeedThree_BCB);

}


void InitializePauseMenusButtons()
{

    AddButton("PauseMenuMainMenuButtonObject", &PauseMenuMainMenu_BCB);
    AddButton("PauseMenuOptionsButtonObject",  &PauseMenuOptions_BCB);
    AddButton("PauseMenuSaveButtonObject",     &PauseMenuSave_BCB);
    AddButton("PauseMenuExitButtonObject",     &PauseMenuExit_BCB);

}

void InitializeOptionsButtons()
{

    AddButton("OptionsMenuOptionsBackButtonObject", &OptionsMenuExit_BCB);
    AddButton("OptionsMenuResolutionButtonObject",  &OptionsMenuResolution_BCB);  

    AddButton("DisplayMenuExitButtonObject",        &DisplayMenuExit_BCB);
    AddButton("DisplayMenuFullScreenButtonObject",  &DisplayMenuFullScreen_BCB);  

}




void InitializeButtons() 
{

    button_callbacks = Vector_Create(sizeof(ButtonCallbacks), 16);

    AddButton("STUB", &StubCallBack);

    InitializeMainMenuButtons();
    InitializeStocksMenuButtons();
    InitializeAccountButtons();
    InitializeGeneralPurposeButtons();
    InitializePauseMenusButtons();
    InitializeOptionsButtons();
    InitializeSaveMenuButtons();
    InitializeCardsMenuButtons();
    InitializeNewsButtons();

}