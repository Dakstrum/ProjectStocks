#ifndef SHARED_H
#define SHARED_H

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

float MaxF(float *array, unsigned int size);
float MinF(float *array, unsigned int size);
float MaxMinDiff(float *array, unsigned int size);

#endif