#include <stdio.h>
#include <stdlib.h>
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

char *GetFormattedPointer(const char *str, ...) 
{
    // NEEDS TO BE FREED, so be sure to free the return.
    char *buffer = malloc(sizeof(char) * 2048);

    va_list args;
    va_start(args, str);
    vsprintf(buffer, str, args);
    va_end(args);
    
    buffer[2047] = '\0';

    return buffer;

}


float MaxF(float *array, unsigned int size) 
{

    float max = 0;
    for (unsigned int i = 0;i < size;i++) {

        if (max < array[i])
            max = array[i];

    }
    return max;

}

float MinF(float *array, unsigned int size) 
{

    float min = 0;
    for (unsigned int i = 0;i < size;i++) {

        if (min > array[i])
            min = array[i];

    }
    return min;

}

float MaxMinDiff(float *array, unsigned int size) 
{

    return MaxF(array, size) - MinF(array, size);
    
}