
#include <string.h>

#include "log.h"

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
        
    return &StubCallback;

}

void StubCallback() 
{

    Log("STUB: StubCallback");

}