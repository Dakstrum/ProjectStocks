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


void StubCallback();

void MainMenuStartButtonCallBack();
void MainMenuOptionsButtonCallBack();
void MainMenuExitButtonCallBack();
void OptionsMenuExitButtonCallBack();
void StocksButtonCallBack();
void NewsButtonCallBack();
void AccountButtonCallBack();
void EavesdropperButtonCallBack();



typedef struct ButtonCallsbacks 
{

    char *name;
    void (*Callback)();

} ButtonCallsbacks;

#define NUM_CALLBACKS 9
static ButtonCallsbacks callbacks[] = {

    {"STUB",                  &StubCallback},
    {"StartButton",           &MainMenuStartButtonCallBack},
    {"OptionsButton",         &MainMenuOptionsButtonCallBack},
    {"ExitButton",            &MainMenuExitButtonCallBack},
    {"OptionsMenuExitButton", &OptionsMenuExitButtonCallBack},
    {"StocksButton",          &StocksButtonCallBack},
    {"NewsButton",            &NewsButtonCallBack},
    {"AccountButton",         &AccountButtonCallBack},
    {"EavesdroppperButton",   &EavesdropperButtonCallBack}


};

void *GetButtonCallback(char *button_name) 
{

    for (int i = 0; i < NUM_CALLBACKS; i++) 
        if (strcmp(button_name, callbacks[i].name) == 0) 
            return callbacks[i].Callback;

    LogF("Unable to find callback for %s", button_name);
    return &StubCallback;

}

void StubCallback() 
{
    Log("STUB: Button may have not been set correctly StubCallback");
}


//Main Menu Callbacks
void MainMenuStartButtonCallBack()
{
    InitStartGame();
}

void MainMenuOptionsButtonCallBack()
{
    ToggleOptionsMenu();
}

void MainMenuExitButtonCallBack()
{
    SetCleanUpToTrue();
}

//Options Menu Callbacks
void OptionsMenuExitButtonCallBack()
{
    ToggleOptionsMenu();
}

//Navigation Bar Callbacks
void StocksButtonCallBack()
{
    ClearCurrentDrawLayer();
    InitializeStocksMenu();
}

void NewsButtonCallBack()
{
    ClearCurrentDrawLayer();
    InitializeNewsMenu();
}

void AccountButtonCallBack()
{
    ClearCurrentDrawLayer();
    InitializeAccountMenu();
}

void EavesdropperButtonCallBack()
{
    Log("Eaves");
}