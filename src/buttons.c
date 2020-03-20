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


typedef struct ButtonCallbacks 
{

    char *name;
    void (*Callback)();

} ButtonCallbacks;


static ButtonCallbacks *callbacks   = NULL;
static unsigned short num_callbacks = 0;
static unsigned short callback_size = 16;
static const unsigned short CALLBACK_SIZE_INCREMENT = 16;

void StubCallBack() 
{

    Log("STUB: Button may have not been set correctly StubCallback");

}

void *GetButtonCallback(char *button_name) 
{

    for (int i = 0; i < num_callbacks; i++) 
        if (strcmp(button_name, callbacks[i].name) == 0) 
            return callbacks[i].Callback;

    return &StubCallBack;

}

void AddButton(char *button_name, void (*Callback)())
{

    if (num_callbacks == callback_size) {

        callback_size += CALLBACK_SIZE_INCREMENT;
        callbacks      = realloc(callbacks, sizeof(ButtonCallbacks) * callback_size);

    }
    callbacks[num_callbacks].name     = button_name;
    callbacks[num_callbacks].Callback = Callback;
    num_callbacks++;

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
    AddButton("PauseMenuMainMenuButton", &PauseMenuMainMenu_BCB);
    AddButton("PauseMenuOptionsButton", &PauseMenuOptions_BCB);
    AddButton("PauseMenuExitButton", &PauseMenuExit_BCB);
    AddButton("OptionsMenuExitButton", &OptionsMenuExit_BCB);

    AddButton("PauseSpeedButton", &PauseSpeed_BCB);
    AddButton("SpeedOneButton", &SpeedOne_BCB);
    AddButton("SpeedTwoButton", &SpeedTwo_BCB);
    AddButton("SpeedThreeButton", &SpeedThree_BCB);

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

    callbacks = malloc(sizeof(ButtonCallbacks) * CALLBACK_SIZE_INCREMENT);

    AddButton("STUB", &StubCallBack);

    InitializeMainMenuButtons();
    InitializeStocksMenuButtons();
    InitializeAccountButtons();
    InitializeGeneralPurposeButtons();
    InitializeSaveMenuButtons();

}