#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdarg.h>

#include "log.h"

static bool should_clean_up = false;

bool ShouldICleanUp() 
{

    return should_clean_up;

}

void SetCleanUpToTrue() 
{

    should_clean_up = true;

}
