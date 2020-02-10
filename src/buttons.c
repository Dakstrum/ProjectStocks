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


typedef struct ButtonCallsbacks 
{

    char *name;
    void (*Callback)();

} ButtonCallsbacks;


void StubCallBack() 
{

    Log("STUB: Button may have not been set correctly StubCallback");

}

#define NUM_CALLBACKS 33
static ButtonCallsbacks callbacks[] = {

    {"STUB",                               &StubCallBack},

    //Main Menu
    {"StartButton",                        &Start_BCB},
    {"OptionsButton",                      &MainMenuOptions_BCB},
    {"ExitButton",                         &MainMenuExit_BCB},
    {"GitHubButton",                       &GitHub_BCB},
    {"TwitterButton",                      &Twitter_BCB},
    {"YoutubeButton",                      &Youtube_BCB},
    {"WebsiteButton",                      &Website_BCB},
    
    //Stocks Menu
    {"ToggleTransactionSellButton",        &SellMenu_BCB},
    {"ToggleTransactionBuyButton",         &BuyMenu_BCB},
    {"MakeTransactionSellButton",          &Sell_BCB},
    {"MakeTransactionBuyButton",           &Buy_BCB},
    {"OneDButton",                         &OneD_BCB},
    {"OneWButton",                         &OneW_BCB},
    {"OneMButton",                         &OneM_BCB},
    {"OneYButton",                         &OneY_BCB},
    {"AllButton",                          &All_BCB},

    //General Purpose
    {"StocksButton",                       &Stocks_BCB},
    {"NewsButton",                         &News_BCB},
    {"AccountButton",                      &Account_BCB},
    {"CardsButton",                        &Cards_BCB},
    {"PauseMenuMainMenuButton",            &PauseMenuMainMenu_BCB},
    {"PauseMenuOptionsButton",             &PauseMenuOptions_BCB},
    {"PauseMenuExitButton",                &PauseMenuExit_BCB},
    {"OptionsMenuExitButton",              &OptionsMenuExit_BCB},

    //Save Menus
    {"LoadSaveMenuNewSaveButton",          &NewSaveMenu_BCB},
    {"NewSaveMenuBackButton",              &NewSaveMenuBack_BCB},
    {"NewSaveMenuCreateButton",            &CreateSave_BCB},
    {"LoadSaveMenuLoadButton",             &LoadSaveMenuLoad_BCB},
    {"LoadSaveMenuBackButton",             &LoadSaveMenuBack_BCB},
    {"LoadSaveMenuDeleteSaveButton",       &DeleteSave_BCB},

    //Cards Menu
    {"TempApplyButton",                    &TempApply_BCB},
    {"CardApplyExitButton",                &CardApplyExit_BCB}
    
};


void *GetButtonCallback(char *button_name) 
{

    for (int i = 0; i < NUM_CALLBACKS; i++) 
        if (strcmp(button_name, callbacks[i].name) == 0) 
            return callbacks[i].Callback;

    return &StubCallBack;

}