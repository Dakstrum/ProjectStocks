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


void StubCallback() 
{

    Log("STUB: Button may have not been set correctly StubCallback");

}

#define NUM_CALLBACKS 33
static ButtonCallsbacks callbacks[] = {

    {"STUB",                               &StubCallback},
    {"StartButton",                        &MainMenuStartButtonCallBack},
    {"OptionsButton",                      &MainMenuOptionsButtonCallBack},
    {"ExitButton",                         &MainMenuExitButtonCallBack},
    {"PauseMenuMainMenuButton",            &PauseMenuMainMenuButtonCallBack},
    {"PauseMenuOptionsButton",             &PauseMenuOptionsButtonCallBack},
    {"PauseMenuExitButton",                &PauseMenuExitButtonCallBack},
    {"OptionsMenuExitButton",              &OptionsMenuExitButtonCallBack},
    {"StocksButton",                       &StocksButtonCallBack},
    {"NewsButton",                         &NewsButtonCallBack},
    {"AccountButton",                      &AccountButtonCallBack},
    {"CardsButton",                        &CardsButtonCallBack},
    {"GitHubButton",                       &MainMenuGitHubButtonCallBack},
    {"TwitterButton",                      &MainMenuTwitterButtonCallBack},
    {"YoutubeButton",                      &MainMenuYoutubeButtonCallBack},
    {"WebsiteButton",                      &MainMenuWebsiteButtonCallBack},
    {"ToggleTransactionSellButton",        &StocksSellButtonCallBack},
    {"ToggleTransactionBuyButton",         &StocksBuyButtonCallBack},
    {"MakeTransactionSellButton",          &MakeSellTransactionButtonCallBack},
    {"MakeTransactionBuyButton",           &MakeBuyTransactionButtonCallBack},
    {"LoadSaveMenuNewSaveButton",          &NewSaveButtonCallBack},
    {"NewSaveMenuBackButton",              &NewSaveMenuBackButtonCallBack},
    {"NewSaveMenuCreateButton",            &NewSaveMenuCreateButtonCallBack},
    {"LoadSaveMenuLoadButton",             &LoadSaveMenuLoadButtonCallBack},
    {"TempApplyButton",                    &TempApplyButtonCallBack},
    {"CardApplyExitButton",                &CardApplyExitButtonCallBack},
    {"OneDButton",                         &OneDButtonCallBack},
    {"OneWButton",                         &OneWButtonCallBack},
    {"OneMButton",                         &OneMButtonCallBack},
    {"OneYButton",                         &OneYButtonCallBack},
    {"AllButton",                          &AllButtonCallBack},
    {"LoadSaveMenuBackButton",             &LoadSaveMenuBackButtonCallBack},
    {"LoadSaveMenuDeleteSaveButton",       &LoadSaveMenuDeleteSaveButtonCallBack}

};


void *GetButtonCallback(char *button_name) 
{

    for (int i = 0; i < NUM_CALLBACKS; i++) 
        if (strcmp(button_name, callbacks[i].name) == 0) 
            return callbacks[i].Callback;

    return &StubCallback;

}