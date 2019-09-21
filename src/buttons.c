#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "log.h"

#include "shared.h"

#include "mainmenu.h"

void StubCallback();

void MainMenuStartButtonCallBack();
void MainMenuOptionsButtonCallBack();
void MainMenuExitButtonCallBack();
void OptionsMenuExitButtonCallBack();



typedef struct ButtonCallsbacks 
{

    char *name;
    void (*Callback)();

} ButtonCallsbacks;

#define NUM_CALLBACKS 5
static ButtonCallsbacks callbacks[] = {

    {"STUB",                  &StubCallback},
    {"StartButton",           &MainMenuStartButtonCallBack},
    {"OptionsButton",         &MainMenuOptionsButtonCallBack},
    {"ExitButton",            &MainMenuExitButtonCallBack},
    {"OptionsMenuExitButton", &OptionsMenuExitButtonCallBack}

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
    ShowStartMenu();
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
    Log("OPTIONSMENUEXITBUTTONCALLBACK");
    ToggleOptionsMenu();
}