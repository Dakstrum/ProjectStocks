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

    AddButton("BuyMenuXButtonObject",             &BuyMenu_BCB);

    AddButton("SellMenuXButtonObject",            &SellMenu_BCB);
    
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

}

void InitializeNewsButtons()
{

    AddButton("NewsMenuStocksTabButtonObject",  &Stocks_BCB);
    AddButton("NewsMenuNewsTabButtonObject",    &News_BCB);
    AddButton("NewsMenuAccountTabButtonObject", &Account_BCB);
    AddButton("NewsMenuCardsTabButtonObject",   &Cards_BCB);

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

}


void InitializePauseMenusButtons()
{

    AddButton("PauseMenuMainMenuButton", &PauseMenuMainMenu_BCB);
    AddButton("PauseMenuOptionsButton", &PauseMenuOptions_BCB);
    AddButton("PauseMenuSaveButton", &PauseMenuSave_BCB);
    AddButton("PauseMenuExitButton", &PauseMenuExit_BCB);

}

void InitializeOptionsButtons()
{

    AddButton("OptionsMenuExitButton", &OptionsMenuExit_BCB);
    AddButton("OptionsMenuResolutionButton", &OptionsMenuResolution_BCB);  

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