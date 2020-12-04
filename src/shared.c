#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdarg.h>
#include <float.h>
#include <time.h>
#include <math.h>

#include "log.h"
#include "shared.h"

static atomic_bool should_clean_up;
static const int BUFFER_SIZE = 8192;

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
    char *buffer = malloc(sizeof(char) * BUFFER_SIZE);
    memset(buffer, '\0', BUFFER_SIZE);

    va_list args;
    va_start(args, str);
    vsprintf(buffer, str, args);
    va_end(args);
    
    buffer[BUFFER_SIZE-1] = '\0';

    return buffer;

}

void SetFormattedPointerVaList(char *buffer, const char *str, va_list args) 
{

    vsprintf(buffer, str, args);
    va_end(args);
    
    buffer[BUFFER_SIZE-1] = '\0';

}

char *GetFormattedPointerVaList(const char *str, va_list args)
{

    // NEEDS TO BE FREED, so be sure to free the return.
    char *buffer = malloc(sizeof(char) * BUFFER_SIZE);
    memset(buffer, '\0', BUFFER_SIZE);

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

struct timespec GetTimespecDiff(struct timespec *t1, struct timespec *t2)
{

    struct timespec diff_time;
    long sec_diff  = 0;
    long diff_nsec = 0;
    if (t1->tv_nsec > t2->tv_nsec) {

        sec_diff  = -1;
        diff_nsec = 999999999 - (t1->tv_nsec - t2->tv_nsec); 

    } else {

        diff_nsec = t2->tv_nsec - t1->tv_nsec;

    }

    diff_time.tv_sec  = t2->tv_sec - t1->tv_sec + sec_diff;
    diff_time.tv_nsec = diff_nsec;

    return diff_time;

}

long GetMilliDiff(struct timespec *t1, struct timespec *t2)
{

    return ((t1->tv_sec * 1e3) + (t1->tv_nsec / 1e6)) - ((t2->tv_sec * 1e3) + (t2->tv_nsec / 1e6));    

}

double GetDoubleMilliDiff(struct timespec *t1, struct timespec *t2)
{

    return (double)((t1->tv_sec * 1e3) + (t1->tv_nsec / 1e6)) - ((t2->tv_sec * 1e3) + (t2->tv_nsec / 1e6));    

}

long GetAbsMilliDiff(struct timespec *t1, struct timespec *t2) 
{

    return labs(GetMilliDiff(t1, t2));

}

struct timespec GetCurrentTime() 
{

    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    return current_time;

}

bool IsTimeSpecInPast(struct timespec *t)
{

    struct timespec temp = GetCurrentTime();
    return GetMilliDiff(&temp, t) > 0;

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

float GetSeconds(struct timespec *t) 
{

    return (float)(t->tv_sec + t->tv_nsec * 1e-9);

}

#define RAND48_SEED_0   (0x330e)
#define RAND48_SEED_1   (0xabcd)
#define RAND48_SEED_2   (0x1234)
#define RAND48_MULT_0   (0xe66d)
#define RAND48_MULT_1   (0xdeec)
#define RAND48_MULT_2   (0x0005)
#define RAND48_ADD      (0x000b)

uint16_t _rand48_seed[3] = {
    RAND48_SEED_0,
    RAND48_SEED_1,
    RAND48_SEED_2
};
uint16_t _rand48_mult[3] = {
    RAND48_MULT_0,
    RAND48_MULT_1,
    RAND48_MULT_2
};
uint16_t _rand48_add = RAND48_ADD;

void shared_dorand48(uint16_t xseed[3])
{
    uint64_t accu;
    uint16_t temp[2];

    accu = (uint64_t) _rand48_mult[0] * (uint64_t) xseed[0] + (uint64_t) _rand48_add;
    temp[0] = (uint16_t) accu;        /* lower 16 bits */
    accu >>= sizeof(uint16_t) * 8;
    accu += (uint64_t) _rand48_mult[0] * (uint64_t) xseed[1] + (uint64_t) _rand48_mult[1] * (uint64_t) xseed[0];
    temp[1] = (uint16_t) accu;        /* middle 16 bits */
    accu >>= sizeof(uint16_t) * 8;
    accu += _rand48_mult[0] * xseed[2] + _rand48_mult[1] * xseed[1] + _rand48_mult[2] * xseed[0];
    xseed[0] = temp[0];
    xseed[1] = temp[1];
    xseed[2] = (uint16_t) accu;
}

long shared_nrand48(uint16_t xseed[3]) 
{

    shared_dorand48(xseed);
    return ((long) xseed[2] << 15) + ((long) xseed[1] >> 1);

}

double shared_erand48(uint16_t xseed[3])
{
        shared_dorand48(xseed);
        return ldexp((double) xseed[0], -48) +
               ldexp((double) xseed[1], -32) +
               ldexp((double) xseed[2], -16);
}

uint16_t *shared_seed48(uint16_t xseed[3])
{
    static uint16_t sseed[3];

    sseed[0] = _rand48_seed[0];
    sseed[1] = _rand48_seed[1];
    sseed[2] = _rand48_seed[2];
    _rand48_seed[0] = xseed[0];
    _rand48_seed[1] = xseed[1];
    _rand48_seed[2] = xseed[2];
    _rand48_mult[0] = RAND48_MULT_0;
    _rand48_mult[1] = RAND48_MULT_1;
    _rand48_mult[2] = RAND48_MULT_2;
    _rand48_add = RAND48_ADD;
    return sseed;
}