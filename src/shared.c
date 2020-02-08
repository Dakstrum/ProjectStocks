#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdarg.h>
#include <float.h>
#include <time.h>
#include <math.h>

#include "log.h"
#include "shared.h"

static atomic_bool should_clean_up;
static WindowSettings window_settings;
static const float window_width  = 1.0/1920.0;
static const float window_height = 1.0/1080.0;

void InitializeShared() 
{

    atomic_store(&should_clean_up, false);

}

void SetWindowSettings(WindowSettings settings) 
{

    window_settings = settings;

}

WindowSettings GetWindowSettings() 
{

    return window_settings;    

}

WindowScale GetWindowScale() 
{

    WindowScale scale;
    scale.x_scale = (float)window_settings.width * window_width;
    scale.y_scale = (float)window_settings.height * window_height;

    return scale;

}

bool ShouldICleanUp() 
{

    return atomic_load(&should_clean_up);

}

void SetCleanUpToTrue() 
{

    atomic_store(&should_clean_up, true);

}

char *GetFormattedBuffer(char buffer[1024], const char *str, ...) 
{

    va_list args;
    va_start(args, str);
    vsprintf(buffer, str, args);

    va_end(args);
    buffer[1023] = '\0';
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

void SetFormattedPointerVaList(char *buffer, const char *str, va_list args) 
{

    //va_start(args, str);
    vsprintf(buffer, str, args);
    va_end(args);
    
    buffer[511] = '\0';

}

char *GetFormattedPointerVaList(const char *str, va_list args)
{

    // NEEDS TO BE FREED, so be sure to free the return.
    char *buffer = malloc(sizeof(char) * 512);
    SetFormattedPointerVaList(buffer, str, args);
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

    float min = FLT_MAX;
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

long GetMillDiff(struct timespec *t1, struct timespec *t2) 
{

    return labs( ((t1->tv_sec * 1e3) + (t1->tv_nsec / 1e6)) - ((t2->tv_sec * 1e3) + (t2->tv_nsec / 1e6)) );

}

struct timespec GetCurrentTime() 
{

    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    return current_time;

}

struct timespec GetOffsetTime(long offset_in_milli)
{

    struct timespec offset_time = GetCurrentTime();

    long seconds = offset_in_milli / 1000;
    long milli   = offset_in_milli % 1000;

    // TODO Doesn't account for overflows;
    offset_time.tv_sec  += seconds;
    offset_time.tv_nsec += (milli * 1e6);

    return offset_time;

}