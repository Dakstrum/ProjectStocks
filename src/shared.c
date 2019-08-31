#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdarg.h>

#include "log.h"

static atomic_bool should_clean_up;

void InitializeShared() 
{

    atomic_store(&should_clean_up, false);

}

bool ShouldICleanUp() 
{

    return atomic_load(&should_clean_up);

}

void SetCleanUpToTrue() 
{

    atomic_store(&should_clean_up, true);

}

char *GetFormattedBuffer(char buffer[512], const char *str, ...) 
{

    va_list args;
    va_start(args, str);
    vsprintf(buffer, str, args);

    va_end(args);
    buffer[511] = '\0';
    return buffer;

}
