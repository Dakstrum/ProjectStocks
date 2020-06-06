#ifndef DBUTILS_H
#define DBUTILS_H

#include <sqlite3.h>

char *LoadSqlFile(const char *file);

char *MemoryConnection();
char *DefaultConnection();
int OpenConnection(sqlite3 **db, char *connection_string);
int OpenConnectionCreate(sqlite3 **db, char *connection_string);

void ExecuteQuery(char *query, int (*callback)(void *,int, char**, char **), void *callback_var, sqlite3 *db);
void ExecuteQueryF(int (*callback)(void *,int, char**, char **), void *callback_var, const char *query, ...);
void ExecuteQueryFDB(int (*callback)(void *,int, char**, char **), void *callback_var, sqlite3 *db, const char *query, ...);

#endif