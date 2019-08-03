#include <stdbool.h>

static bool should_clean_up = false;

bool ShouldICleanUp() 
{

    return should_clean_up;

}

void SetCleanUpToTrue() 
{

    should_clean_up = true;

}