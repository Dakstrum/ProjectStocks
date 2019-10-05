#ifndef LOG_H
#define LOG_H

void InitializeLogging();

void Log(const char *str);
void LogF(const char *str, ...);

void LogNoQueue(const char *str);
void LogFNoQueue(const char *str, ...);

#endif