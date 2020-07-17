#ifndef DBACCESS_H
#define DBACCESS_H

#include <sqlite3.h>

#include "graph.h"
#include "shared.h"

void InitializeDatabases(); 
void CleanUpDatabases(); 
void SetupMainDB(); 
void SetupLogDB();


#endif