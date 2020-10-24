
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "log.h"
#include "vector.h"
#include "shared.h"
#include "dbutils.h"

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
    int c;
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

char *MemoryConnection() 
{

    return "file:simdb?mode=memory&cache=shared";

}

char *DefaultConnection() 
{

    return "blinky.db";

}

sqlite3 *GetOpenConnection(char *connection_string)
{

    sqlite3 *db = NULL;
    if (sqlite3_open_v2(connection_string, &db, SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {

        LogF("Could not establish connection to %s", connection_string);
        SetCleanUpToTrue();
        return NULL;

    }
    return db;

}

int OpenConnection(sqlite3 **db, char *connection_string) 
{

    if (sqlite3_open_v2(connection_string, db, SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {

        LogF("Could not establish connection to %s", connection_string);
        SetCleanUpToTrue();
        return -1;

    }
    return 0;

}

int OpenConnectionCreate(sqlite3 **db, char *connection_string) 
{

    if  (sqlite3_open_v2(connection_string, db, SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {

        LogF("Could not establish connection to %s for creation or write/read access.", connection_string);
        SetCleanUpToTrue();
        return -1;

    }
    return 0;

}

void ExecuteQuery(char *query, int (*callback)(void *,int, char**, char **), void *callback_var, sqlite3 *db) 
{

    char *error = NULL;
    sqlite3_exec(db, query, callback, callback_var, &error);

    if (error != NULL)
        LogF("SQL ERROR %s, query = %s", error, query);

    free(query);

}

void ExecuteQueryF(int (*callback)(void *,int, char**, char **), void *callback_var, const char *query, ...)
{

    sqlite3 *db;
    va_list args;
    va_start(args, query);

    if (OpenConnection(&db, DefaultConnection()) != 0) {

        LogF("Could not open connection: %s", query);
        return;

    }

    ExecuteQuery(GetFormattedPointerVaList(query, args), callback, callback_var, db);
    sqlite3_close(db);

}

void ExecuteQueryFDB(int (*callback)(void *,int, char**, char **), void *callback_var, sqlite3 *db, const char *query, ...)
{

    va_list args;
    va_start(args, query);

    ExecuteQuery(GetFormattedPointerVaList(query, args), callback, callback_var, db);

}

void InsertMessage(sqlite3 *db, char *message) 
{

    char *error = NULL;
    sqlite3_exec(db, message, NULL, NULL, &error);

    if (error != NULL)
        LogFNoQueue("SQL ERROR %s, query = %s", error, message);

}

void ExecuteTransaction(sqlite3 *db, Vector *vector)
{

    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, 0, 0);

    char **messages = vector->elements;
    for (size_t i = 0; i < vector->num_elements; i++)
        InsertMessage(db, messages[i]);

    sqlite3_exec(db, "END TRANSACTION", NULL, 0, 0);
    sqlite3_close(db);

}
