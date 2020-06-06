#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sqlite3.h>

#include "log.h"
#include "shared.h"
#include "dbutils.h"
#include "dbaccess.h"

void SetupMainDB();
void SetupLogDB();

void SetWindowSettingsIfExists(void *settings, int argc, char **argv, char **col_name) 
{

    WindowSettings *temp_settings = (WindowSettings *)settings;
    if (argc == 0)
        return;

    temp_settings->width          = atoi(argv[0]);
    temp_settings->height         = atoi(argv[1]);
    temp_settings->screen_flag    = atoi(argv[2]);


}

WindowSettings GetSettingsFromDB(sqlite3 *db) 
{

    WindowSettings settings = {0, 0, WINDOWED};
    ExecuteQuery(GetFormattedPointer("SELECT WindowWidth, WindowHeight, WindowStyle FROM Settings"), (void *)(&SetWindowSettingsIfExists), &settings, db);

    if (settings.width == 0) {

        settings.width  = 1920;
        settings.height = 1080;
        settings.screen_flag = WINDOWED;
        ExecuteQuery(GetFormattedPointer("INSERT INTO Settings (WindowWidth, WindowHeight, WindowStyle) VALUES (1920, 1080, 1);"), NULL, NULL, db);

    }
    return settings;

}

WindowSettings GetWindowSettingsFromDB() 
{

    WindowSettings settings = {1920, 1080, WINDOWED};
    sqlite3 *db = NULL;
    if (OpenConnection(&db, DefaultConnection()))
        return settings;

    return GetSettingsFromDB(db);

}

void SetWindowResolutionSettings(int width, int height)
{

    sqlite3 *db; 
    if (OpenConnection(&db, DefaultConnection()) == 0) 
        ExecuteQuery(GetFormattedPointer("UPDATE Settings SET WindowWidth = %d, WindowHeight = %d WHERE SettingsId = 1;", width, height ), NULL, NULL, db);

    sqlite3_close(db);

}

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
    ExecuteQueryF(&SeedDB_Callback, &seeded, "SELECT * FROM DBEvents WHERE Event = 'Seeded'");
    if (seeded) {
        
        Log("Already seeded db");
        return;

    }

    ExecuteQueryF(NULL, NULL, LoadSqlFile("assets/sql/companies-seed.sql"));
    ExecuteQueryF(NULL, NULL, LoadSqlFile("assets/sql/news-events-seed.sql"));
    ExecuteQueryF(NULL, NULL, "INSERT INTO DBEvents (Event) VALUES ('Seeded')");

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