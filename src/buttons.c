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
void EavesdropperButtonCallBack();
void MainMenuGitHubButtonCallBack();
void MainMenuTwitterButtonCallBack();
void MainMenuYoutubeButtonCallBack();
void MainMenuWebsiteButtonCallBack();



typedef struct ButtonCallsbacks 
{

    char *name;
    void (*Callback)();

} ButtonCallsbacks;

#define NUM_CALLBACKS 15
static ButtonCallsbacks callbacks[] = {

    {"STUB",                    &StubCallback},
    {"StartButton",             &MainMenuStartButtonCallBack},
    {"OptionsButton",           &MainMenuOptionsButtonCallBack},
    {"ExitButton",              &MainMenuExitButtonCallBack},
    {"PauseMenuMainMenuButton", &PauseMenuMainMenuButtonCallBack},
    {"PauseMenuOptionsButton",  &PauseMenuOptionsButtonCallBack},
    {"PauseMenuExitButton",     &PauseMenuExitButtonCallBack},
    {"OptionsMenuExitButton",   &OptionsMenuExitButtonCallBack},
    {"StocksButton",            &StocksButtonCallBack},
    {"NewsButton",              &NewsButtonCallBack},
    {"AccountButton",           &AccountButtonCallBack},
    {"GitHubButton",            &MainMenuGitHubButtonCallBack},
    {"TwitterButton",           &MainMenuTwitterButtonCallBack},
    {"YoutubeButton",           &MainMenuYoutubeButtonCallBack},
    {"WebsiteButton",           &MainMenuWebsiteButtonCallBack}


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
