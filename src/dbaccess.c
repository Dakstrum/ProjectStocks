#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sqlite3.h>

#include "log.h"
#include "graph.h"
#include "shared.h"
#include "dbaccess.h"

void SetupMainDB();
void SetupLogDB();

void InsertDefaultSettingsIfNeeded(sqlite3 *db);
int GetCompanyId(char *company_name, sqlite3 *db);
bool DoesCompanyExist(char *company_name, sqlite3 *db);
void SetCompanyToActive(char *company_name, sqlite3 *db);
void InsertNewCompany(char *company_name, float ipo, sqlite3 *db);
void ExecuteQuery(char *query, int (*callback)(void *,int, char**, char **), void *callback_var, sqlite3 *db);

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

char *MemoryConnection() 
{

    return "file:simdb?mode=memory&cache=shared";

}

char *DefaultConnection() 
{

    return "blinky.db";

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

void BeginInMemoryBackup(sqlite3 *persistent, sqlite3 *memory)
{

    sqlite3_backup *backup;
    if ((backup = sqlite3_backup_init(memory, "main", persistent, "main")) != NULL) {

        sqlite3_backup_step(backup, -1);
        sqlite3_backup_finish(backup);

    } else {

        LogF("Backup error: %s", sqlite3_errmsg(memory));
        SetCleanUpToTrue();

    }

}

void CopyPersistentToMemory() 
{

    sqlite3 *persistent;
    sqlite3 *memory;

    if (OpenConnection(&persistent, DefaultConnection()) == -1)
        return;
    if (OpenConnection(&memory, MemoryConnection()) == -1) {

        sqlite3_close(persistent);
        return;

    }

    BeginInMemoryBackup(persistent, memory);

    sqlite3_close(persistent);
    sqlite3_close(memory);

}

void SetUpDB(sqlite3 **db, char *connection_string) 
{

    if (OpenConnectionCreate(db, connection_string) == -1)
        return;

    char *error       = NULL;
    char *drop_stocks = "DROP TABLE IF EXISTS StockPrices;";
    sqlite3_exec(*db, drop_stocks, NULL, 0, NULL);

    char *setup =   "CREATE TABLE IF NOT EXISTS Company(CompanyId INTEGER PRIMARY KEY, Ipo DOUBLE NOT NULL, CompanyName VARCHAR(50) NOT NULL, Category VARCHAR(30), IsActiveInJson INT DEFAULT 0);"
                    "CREATE TABLE IF NOT EXISTS CompanyMetadata(CompanyMetaId INTEGER PRIMARY KEY, CompanyId INT NOT NULL, TotalEmployees INT NOT NULL, StocksInMarket UNSIGNED BIG INT, SaveId INT NOT NULL);"
                    "CREATE TABLE IF NOT EXISTS Players(PlayerId INTERGER PRIMARY KEY, SaveId INT NOT NULL, PlayerName TEXT NOT NULL, Money DOUBLE NOT NULL);"
                    "CREATE TABLE IF NOT EXISTS Saves(SaveId INTEGER PRIMARY KEY, SaveName TEXT NOT NULL, PlayerName TEXT NOT NULL, TimeSpentInGame UNSIGNED BIG INT DEFAULT(0), RandomSeed UNSIGNED BIG INT);"
                    "CREATE TABLE IF NOT EXISTS OwnedStocks(OwnedStockId INTEGER PRIMARY KEY, SaveId INT NOT NULL, PlayerName TEXT NOT NULL, CompanyId INT NOT NULL, HowManyOwned UNSIGNED BIG INT NOT NULL);"
                    "CREATE TABLE IF NOT EXISTS Transactions(TransactionId INTEGER PRIMARY KEY, SaveId INT NOT NULL, PlayerName TEXT NOT NULL, CompanyId INT NOT NULL, TransactionAmount DOUBLE NOT NULL, StocksExchanged INT NOT NULL, TransactionTime DATETIME NOT NULL);"
                    "CREATE TABLE IF NOT EXISTS Settings(SettingsId INTEGER PRIMARY KEY, WindowWidth UNSIGNED INT NOT NULL, WindowHeight UNSIGNED INT NOT NULL, WindowStyle UNSIGNED INT NOT NULL);"
                    "UPDATE Company SET IsActiveInJson=0;";
    sqlite3_exec(*db, setup, NULL, 0, &error);
    if (error != NULL)
        LogF("Error setting up db %s with %s", connection_string, error);

}

void SetupMainDB() 
{

    sqlite3 *db = NULL;
    SetUpDB(&db, DefaultConnection());
    sqlite3_close(db);
    
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
    ExecuteQuery(GetFormattedPointer("SELECT WindowWidth, WindowHeight, WindowStyle FROM Settings;"), (void *)(&SetWindowSettingsIfExists), &settings, db);
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

int GetSaveIdCallback(void *save_id, int argc, char **argv, char **col_name) 
{

    if (argc > 0)
        *((int *)save_id) = (unsigned int)atoi(argv[0]);

    return 0;

}

int InsertSave(char *save_name, char *player_name, unsigned int game_seed)
{

    int save_id = -1;

    sqlite3 *db;
    if (OpenConnection(&db, DefaultConnection()) == 0) {

        ExecuteQuery(GetFormattedPointer("INSERT INTO Saves (SaveName, PlayerName, RandomSeed) VALUES ('%s', '%s', %d)", save_name, player_name, game_seed), NULL, NULL, db);
        ExecuteQuery(GetFormattedPointer("SELECT SaveId FROM Saves WHERE RandomSeed = %d AND SaveName = '%s'", game_seed, save_name ), &GetSaveIdCallback, &save_id, db);

    }

    sqlite3_close(db);

    return save_id;

}

int GetSaveSeedCallback(void *seed, int argc, char **argv, char **col_name) 
{

    if (argc > 0)
        *((unsigned int *)seed) = (unsigned int)atoi(argv[0]);

    return 0;

}

unsigned int GetSaveSeedWithSaveId(int save_id)
{

    unsigned int seed = 0;

    sqlite3 *db;
    if (OpenConnection(&db, DefaultConnection()) == 0)
        ExecuteQuery(GetFormattedPointer("SELECT RandomSeed FROM Saves WHERE SaveId=%d", save_id), &GetSaveSeedCallback, &seed, db);

    return seed;

}

int InsertAndOrSetCompanyToActive(char *company_name, float ipo) 
{

    int company_id = -1;

    sqlite3 *db;
    if  (OpenConnection(&db, DefaultConnection()) == 0) {

        if (!DoesCompanyExist(company_name, db))
            InsertNewCompany(company_name, ipo, db);
        
        SetCompanyToActive(company_name, db);
        company_id = GetCompanyId(company_name, db);

    }
    sqlite3_close(db);

    return company_id;

}

int FindOutIfCompanyExists(void *exists, int argc, char **argv, char **col_name) 
{

    if (argc > 0)
        *((bool *)exists) = true;
    
    return 0;

}

bool DoesCompanyExist(char *company_name, sqlite3 *db) 
{

    bool exists = false;
    ExecuteQuery(GetFormattedPointer("SELECT CompanyId FROM Company WHERE CompanyName='%s';", company_name), &FindOutIfCompanyExists, &exists, db);

    return exists;

}

void SetCompanyToActive(char *company_name, sqlite3 *db) 
{

    ExecuteQuery(GetFormattedPointer("UPDATE Company SET IsActiveInJson=1 WHERE CompanyName='%s';", company_name), NULL, NULL, db);

}

void InsertNewCompany(char *company_name, float ipo, sqlite3 *db) 
{

    ExecuteQuery(GetFormattedPointer("INSERT INTO Company (Ipo, CompanyName, IsActiveInJson) VALUES (%f, '%s', 1);", ipo, company_name), NULL, NULL, db);

}

void AddOwnedStock(int owned_stock_id, int save_id, int company_id, int *how_many_owned) 
{

    sqlite3 *db;
    if (OpenConnection(&db, DefaultConnection()) == 0) {

        ExecuteQuery(GetFormattedPointer("INSERT INTO OwnedStocks (OwnedStockId, SaveId, PlayerName, CompanyId, HowManyOwned) VALUES (2, 1, 1, %d, '%d');", company_id, how_many_owned), NULL, NULL, db);
    }

}

void InsertStockTransaction(int transation_id, int save_id, int player_name, int company_id, int *transaction_amount, int stocks_exchanged, time_t transaction_time) 
{

    sqlite3 *db;
    if (OpenConnection(&db, DefaultConnection()) == 0) {

        ExecuteQuery(GetFormattedPointer("INSERT INTO Transactions ( SaveId, PlayerName, CompanyId, TransactionAmount, StocksExchanged, TransactionTime) VALUES (1, 1, 1, '1', %d, %d);", transaction_amount, transaction_time), NULL, NULL, db);
    }

}

void SubtractOwnedStock(int amount_to_subtract)
{
    
    sqlite3 *db;
    if (OpenConnection(&db, DefaultConnection()) == 0) {

        ExecuteQuery(GetFormattedPointer("SELECT * FROM OwnedStocks; UPDATE OwnedStocks SET HowManyOwned = HowManyOwned - %d  WHERE OwnedStockId=2;", amount_to_subtract), NULL, NULL, db);
    }
}

int SetCompanyId(void *company_id, int argc, char **argv, char **col_name) 
{

    if (argc == 0)
        *((int *)company_id) = -1;
    else
        *((int *)company_id) = atoi(argv[0]);

    return 0;

}

int GetCompanyId(char *company_name, sqlite3 *db) 
{

    int company_id;
    ExecuteQuery(GetFormattedPointer("SELECT CompanyId FROM Company WHERE CompanyName='%s'", company_name), &SetCompanyId, &company_id, db);

    return company_id;

}

void InsertStockPrice(int save_id, int company_id, float stock_price, char *timestamp, sqlite3 *db) 
{

    ExecuteQuery(GetFormattedPointer("INSERT INTO StockPrices (SaveId, CompanyId, Price, Time) VALUES (%d, %d, %f, '%s')", save_id, company_id, stock_price, timestamp), NULL, NULL, db);

}

void ExecuteQuery(char *query, int (*callback)(void *,int, char**, char **), void *callback_var, sqlite3 *db) 
{

    char *error = NULL;
    sqlite3_exec(db, query, callback, callback_var, &error);

    if (error != NULL)
        LogF("SQL ERROR %s, query = %s", error, query);

    free(query);

}