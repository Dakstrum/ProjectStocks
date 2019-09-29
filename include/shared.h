#ifndef SHARED_H
#define SHARED_H

void InitializeShared();
bool ShouldICleanUp();
void SetCleanUpToTrue();

char *GetFormattedBuffer(char buffer[512], const char *str, ...);

#endif