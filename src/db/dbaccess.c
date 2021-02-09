#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sqlite3.h>

#include "log.h"
#include "dbutils.h"
#include "dbaccess.h"

void SetupMainDB();
void SetupLogDB();

void InitializeDatabases() 
{

    sqlite3_shutdown();
    sqlite3_config(SQLITE_CONFIG_SERIALIZED);
    sqlite3_initialize();
    SetupMainDB();
    SetupLogDB();
    
}

void CleanUpDatabases() 
{

    sqlite3_shutdown();

}

void SetUpDB() 
{

    sqlite3 *db = NULL;
    if (OpenConnectionCreate(&db, DefaultConnection()) == -1)
        return;

    char *error = NULL;
    sqlite3_exec(db, LoadSqlFile("assets/sql/tables.sql"), NULL, NULL, &error);

    if (error != NULL)
        LogF("SQL ERROR %s when creating database/tables", error);
 
    sqlite3_close(db);

}

int SeedDB_Callback(void *seeded, int argc, char **argv, char **col_name)
{

    if (argc > 0)
        *((bool *)seeded) = true;

    return 0;

}

void SeedDB()
{

    bool seeded = false;
    ExecuteQueryF(&SeedDB_Callback, &seeded, "SELECT * FROM DB_Events WHERE Event = 'Seeded'");
    if (seeded) {
        
        Log("Already seeded db");
        return;

    }
    
    ExecuteQueryF(NULL, NULL, LoadSqlFile("assets/sql/seed.sql"));
    ExecuteQueryF(NULL, NULL, LoadSqlFile("assets/sql/settings-seed.sql"));
    ExecuteQueryF(NULL, NULL, "INSERT INTO DB_Events (Event) VALUES ('Seeded')");

}

void SetupMainDB() 
{

    SetUpDB();
    SeedDB();
    
}

void SetupLogDB() 
{

    sqlite3 *db;
    if  (sqlite3_open_v2("log.db", &db, SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL) == SQLITE_OK) {

        char *setup = "CREATE TABLE IF NOT EXISTS LOGS(LogId INTEGER PRIMARY KEY, TimeStamp DATETIME DEFAULT(datetime(CURRENT_TIMESTAMP, 'localtime')), Log TEXT NOT NULL);";
        sqlite3_exec(db, setup, NULL, 0, 0);

#if DEBUGGING

        char *delete_contents = "DELETE FROM LOGS;";
        sqlite3_exec(db, delete_contents, NULL, 0, 0);

#endif

    }
    sqlite3_close(db);

}