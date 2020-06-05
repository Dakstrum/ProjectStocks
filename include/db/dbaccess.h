#ifndef DBACCESS_H
#define DBACCESS_H

#include <sqlite3.h>

#include "graph.h"
#include "shared.h"

void InitializeDatabases(); 
void CleanUpDatabases(); 
char *MemoryConnection();
char *DefaultConnection(); 
int OpenConnection(sqlite3 **db, char *connection_string); 
int OpenConnectionCreate(sqlite3 **db, char *connection_string); 
void BeginInMemoryBackup(sqlite3 *persistent, sqlite3 *memory);
void CopyPersistentToMemory();
void SetupMainDB(); 
void SetupLogDB();
void SetWindowSettingsIfExists(void *settings, int argc, char **argv, char **col_name);
WindowSettings GetSettingsFromDB(sqlite3 *db);
WindowSettings GetWindowSettingsFromDB();
void SetWindowResolutionSettings(int width, int height);

void ExecuteQuery(char *query, int (*callback)(void *,int, char**, char **), void *callback_var, sqlite3 *db);
void ExecuteQueryF(int (*callback)(void *,int, char**, char **), void *callback_var, const char *query, ...);
void ExecuteQueryFDB(int (*callback)(void *,int, char**, char **), void *callback_var, sqlite3 *db, const char *query, ...);

#endif