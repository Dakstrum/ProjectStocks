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


void StubCallback();

void MainMenuStartButtonCallBack();
void MainMenuOptionsButtonCallBack();
void MainMenuExitButtonCallBack();
void PauseMenuMainMenuButtonCallBack();
void PauseMenuOptionsButtonCallBack();
void PauseMenuExitButtonCallBack();
void OptionsMenuExitButtonCallBack();
void StocksButtonCallBack();
void NewsButtonCallBack();
void AccountButtonCallBack();
void MainMenuGitHubButtonCallBack();
void MainMenuTwitterButtonCallBack();
void MainMenuYoutubeButtonCallBack();
void MainMenuWebsiteButtonCallBack();
void StocksSellButtonCallBack();
void StocksBuyButtonCallBack();
void MakeSellTransactionButtonCallBack();
void MakeBuyTransactionButtonCallBack();
void NewSaveButtonCallBack();
void NewSaveMenuBackButtonCallBack();
void NewSaveMenuCreateButtonCallBack();
void LoadSaveMenuLoadButtonCallBack();

void OneDButtonCallBack();
void OneWButtonCallBack();
void OneMButtonCallBack();
void OneYButtonCallBack();
void AllButtonCallBack();

typedef struct ButtonCallsbacks 
{

    char *name;
    void (*Callback)();

} ButtonCallsbacks;

#define NUM_CALLBACKS 28
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
    {"GitHubButton",                       &MainMenuGitHubButtonCallBack},
    {"TwitterButton",                      &MainMenuTwitterButtonCallBack},
    {"YoutubeButton",                      &MainMenuYoutubeButtonCallBack},
    {"WebsiteButton",                      &MainMenuWebsiteButtonCallBack},
    {"ToggleTransactionSellButton",        &StocksSellButtonCallBack},
    {"ToggleTransactionBuyButton",         &StocksBuyButtonCallBack},
    {"MakeTransactionSellButton",          &MakeSellTransactionButtonCallBack},
    {"MakeTransactionBuyButton",           &MakeBuyTransactionButtonCallBack},
    {"NewSaveButton",                      &NewSaveButtonCallBack},
    {"NewSaveMenuBackButton",              &NewSaveMenuBackButtonCallBack},
    {"NewSaveMenuCreateButton",            &NewSaveMenuCreateButtonCallBack},
    {"LoadSaveMenuLoadButton",             &LoadSaveMenuLoadButtonCallBack},

    {"OneDButton",                         &OneDButtonCallBack},
    {"OneWButton",                         &OneWButtonCallBack},
    {"OneMButton",                         &OneMButtonCallBack},
    {"OneYButton",                         &OneYButtonCallBack},
    {"AllButton",                          &AllButtonCallBack}

};

void *GetButtonCallback(char *button_name) 
{

    for (int i = 0; i < NUM_CALLBACKS; i++) 
        if (strcmp(button_name, callbacks[i].name) == 0) 
            return callbacks[i].Callback;

    return &StubCallback;

}

void StubCallback() 
{

    Log("STUB: Button may have not been set correctly StubCallback");

}
