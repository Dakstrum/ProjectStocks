#ifndef DBACCESS_H
#define DBACCESS_H

#include <sqlite3.h>

#include "graph.h"
#include "shared.h"

void InitializeDatabases(); 
void CleanUpDatabases(); 
void SetupMainDB(); 
void SetupLogDB();
void SetWindowSettingsIfExists(void *settings, int argc, char **argv, char **col_name);
WindowSettings GetSettingsFromDB(sqlite3 *db);
WindowSettings GetWindowSettingsFromDB();
void SetWindowResolutionSettings(int width, int height);


#endif