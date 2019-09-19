
#include <string.h>

#include "log.h"
#include "mainmenu.h"

void StubCallback();

typedef struct ButtonCallsbacks 
{

    char *name;
    void (*Callback)();

} ButtonCallsbacks;

#define NUM_CALLBACKS 1
static ButtonCallsbacks callbacks[] = {
    {"STUB", &StubCallback}
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