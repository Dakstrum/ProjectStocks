#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdarg.h>

#include "log.h"

static atomic_bool should_clean_up           = false;
static atomic_bool should_clean_up_rendering = false;

bool ShouldICleanUp() 
{

    return atomic_load(&should_clean_up);

}

bool ShouldICleanUpDisplay() 
{

    return atomic_load(&should_clean_up_rendering);

}

void SetCleanUpToTrue() 
{

    atomic_store(&should_clean_up, true);

}

void SetCleanUpDisplay() 
{

    atomic_store(&should_clean_up_rendering, true);

}
