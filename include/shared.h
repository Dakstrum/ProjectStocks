#ifndef SHARED_H
#define SHARED_H

#include <time.h>

typedef enum ScreenSetting {

    FULLSCREEN,
    WINDOWED,
    BORDERLESS

} ScreenSetting;

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

typedef struct StockPrices {

    float *prices;
    time_t *times;
    unsigned int num_prices;
    unsigned int size;

} StockPrices;

typedef struct WindowSettings {

    int width;
    int height;
    ScreenSetting screen_flag;
    
} WindowSettings;

typedef struct WindowScale {

    float x_scale;
    float y_scale;

} WindowScale;

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

long GetMillDiff(struct timespec *t1, struct timespec *t2);
struct timespec GetCurrentTime();
struct timespec GetOffsetTime(long offset_in_milli);

void SetWindowSettings(WindowSettings settings);
WindowSettings GetWindowSettings();
WindowScale GetWindowScale();

#endif