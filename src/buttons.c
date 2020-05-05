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

    for (int i = 0; i < button_callbacks->num_elements; i++) 
        if (strcmp(button_name, callbacks[i].name) == 0) 
            return callbacks[i].Callback;

    return &StubCallBack;

}

void AddButton(char *button_name, void (*Callback)())
{

    ButtonCallbacks button_callback = {button_name, Callback};
    PushBack(button_callbacks, &button_callback);

}

void InitializeMainMenuButtons() 
{

    AddButton("StartButton", &Start_BCB);
    AddButton("OptionsButton", &MainMenuOptions_BCB);
    AddButton("ExitButton", &MainMenuExit_BCB);
    AddButton("GitHubButton", &GitHub_BCB);
    AddButton("TwitterButton", &Twitter_BCB);
    AddButton("YoutubeButton", &Youtube_BCB);
    AddButton("WebsiteButton", &Website_BCB);

}

void InitializeStocksMenuButtons()
{

    AddButton("ToggleTransactionSellButton", &SellMenu_BCB);
    AddButton("ToggleTransactionBuyButton", &BuyMenu_BCB);
    AddButton("MakeTransactionSellButton", &Sell_BCB);
    AddButton("MakeTransactionBuyButton", &Buy_BCB);
    AddButton("OneDButton", &OneD_BCB);
    AddButton("OneWButton", &OneW_BCB);
    AddButton("OneMButton", &OneM_BCB);
    AddButton("OneYButton", &OneY_BCB);
    AddButton("AllButton", &All_BCB);

}

void InitializeAccountButtons()
{

    AddButton("AccountDownButton", &AccountDown_BCB);
    AddButton("AccountUpButton", &AccountUp_BCB);

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

void InitializeSaveMenuButtons()
{

    AddButton("LoadSaveMenuNewSaveButton", &NewSaveMenu_BCB);
    AddButton("NewSaveMenuBackButton", &NewSaveMenuBack_BCB);
    AddButton("NewSaveMenuCreateButton", &CreateSave_BCB);
    AddButton("LoadSaveMenuLoadButton", &LoadSaveMenuLoad_BCB);
    AddButton("LoadSaveMenuBackButton", &LoadSaveMenuBack_BCB);
    AddButton("LoadSaveMenuDeleteSaveButton", &DeleteSave_BCB);

}

void InitializeCardsMenuButtons()
{

    AddButton("TempApplyButton", &TempApply_BCB);
    AddButton("CardApplyExitButton", &CardApplyExit_BCB);

}

void InitializeButtons() 
{

    button_callbacks = CreateVector(sizeof(ButtonCallbacks), 16);

    AddButton("STUB", &StubCallBack);

    InitializeMainMenuButtons();
    InitializeStocksMenuButtons();
    InitializeAccountButtons();
    InitializeGeneralPurposeButtons();
    InitializePauseMenusButtons();
    InitializeOptionsButtons();
    InitializeSaveMenuButtons();
    InitializeCardsMenuButtons();

}