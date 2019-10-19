#ifndef SHARED_H
#define SHARED_H

void InitializeShared();
bool ShouldICleanUp();
void SetCleanUpToTrue();

char *GetFormattedBuffer(char buffer[512], const char *str, ...);
char *GetFormattedPointer(const char *str, ...);

float MaxF(float *array, unsigned int size);
float MinF(float *array, unsigned int size);
float MaxMinDiff(float *array, unsigned int size);

#endif