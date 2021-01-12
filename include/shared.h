#ifndef SHARED_H
#define SHARED_H

#include <time.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

typedef enum TimeSpan {

    ONE_DAY = 0,
    THREE_DAYS,
    ONE_WEEK,
    TWO_WEEKS,
    ONE_MONTH,
    THREE_MONTHS,
    SIX_MONTHS,
    ONE_YEAR,
    TWO_YEARS,
    FIVE_YEARS,
    ALL_TIME

} TimeSpan;

void InitializeShared();
bool ShouldICleanUp();
void SetCleanUpToTrue();

char *GetFormattedBuffer(char buffer[512], const char *str, ...);
char *GetFormattedPointer(const char *str, ...);
void SetFormattedPointerVaList(char *buffer, const char *str, va_list args);
char *GetFormattedPointerVaList(const char *str, va_list args);

float MaxF(float *array, unsigned int size);
float MinF(float *array, unsigned int size);
float MaxMinDiff(float *array, unsigned int size);

bool IsTimeSpecInPast(struct timespec *t);
long GetMilliDiff(struct timespec *t1, struct timespec *t2);
double GetDoubleMilliDiff(struct timespec *t1, struct timespec *t2);
long GetAbsMilliDiff(struct timespec *t1, struct timespec *t2);
struct timespec GetCurrentTime();
struct timespec GetOffsetTime(long offset_in_milli);
struct timespec GetTimespecDiff(struct timespec *t1, struct timespec *t2);
float GetSeconds(struct timespec *t);

long shared_nrand48(uint16_t xseed[3]);
uint16_t *shared_seed48(uint16_t xseed[3]);
float shared_get_random_float(uint16_t seed[3]);
float shared_get_random_sign(uint16_t seed[3]);

#endif