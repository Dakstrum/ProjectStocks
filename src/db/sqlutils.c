
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "log.h"

bool DoesFileExist(const char *file)
{

    return access(file, F_OK) != -1;

}

char *ReadSqlFile(const char *file)
{

    FILE *fp = fopen(file, "r");

    if (fp == NULL) {

        LogF("Received null filepointer for %s", file);
        return NULL;

    }

    int i = 0;
    char c;
    char *buffer = malloc(8192);
    memset(buffer, '\0', 8192);

    while ((c = fgetc(fp)) != EOF) {

        buffer[i] = c;
        i++;

    }

    fclose(fp);

    return buffer;

}

char *LoadSqlFile(const char *file) 
{

    if (!DoesFileExist(file)) {

        LogF("%s file does not exist", file);
        return NULL;

    }
    return ReadSqlFile(file);
}
