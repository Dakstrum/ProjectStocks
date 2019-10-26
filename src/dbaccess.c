#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sqlite3.h>

#include "log.h"
#include "graph.h"
#include "dbaccess.h"
#include "shared.h"

void SetupMainDB();
void SetupLogDB();

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
                    "CREATE TABLE IF NOT EXISTS StockPrices(StockPriceId INTEGER PRIMARY KEY, SaveId INT NOT NULL,  CompanyId INT NOT NULL, Price FLOAT NOT NULL, Time DATETIME NOT NULL);"
                    "CREATE TABLE IF NOT EXISTS Saves(SaveId INTEGER PRIMARY KEY, SaveName TEXT NOT NULL, Money DOUBLE NOT NULL, TimeSpentInGame UNSIGNED BIG INT, RandomSeed UNSIGNED BIG INT);"
                    "CREATE TABLE IF NOT EXISTS OwnedStocks(OwnedStockId INTEGER PRIMARY KEY, SaveId INT NOT NULL, CompanyId INT NOT NULL, HowManyOwned UNSIGNED BIG INT NOT NULL);"
                    "CREATE TABLE IF NOT EXISTS Transactions(TransactionId INTEGER PRIMARY KEY, CompanyId INT NOT NULL, TransactionAmount DOUBLE NOT NULL, StocksExchanged INT NOT NULL, TransactionTime DATETIME NOT NULL, SaveId INT NOT NULL)";
    sqlite3_exec(*db, setup, NULL, 0, &error);
    if (error != NULL)
        LogF("Error setting up db %s with %s", connection_string, error);

    char *set_all_companies_to_inactive = "UPDATE Company SET IsActiveInJson=0;";
    sqlite3_exec(*db, set_all_companies_to_inactive, NULL, 0, 0);

}

void SetupMainDB() 
{

    sqlite3 *db = NULL;
    SetUpDB(&db, DefaultConnection());
    sqlite3_close(db);


    sqlite3 *persistent_memory_db = NULL;
    SetUpDB(&persistent_memory_db, MemoryConnection());
    if (persistent_memory_db == NULL)
        Log("Persistent memory never set");
    
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

int SetCompanyPrices(void *prices, int argc, char **argv, char **col_name) {

    StockPrices *prices_temp = (StockPrices *)prices;

    if (argc == 0 )
        return 0;

    if (prices_temp->num_prices == prices_temp->size){

        prices_temp->size  += 128;
        prices_temp->prices = realloc(prices_temp->prices, sizeof(float) * prices_temp->size);

    }

    prices_temp->prices[prices_temp->num_prices] = (float)atof(argv[0]);
    prices_temp->num_prices++;

    return 0;

}

char *GetStockPricesBetweenRangeHourly(char *company_name, char *start_time, char *end_time) 
{

    return GetFormattedPointer(
        "SELECT SP.Price FROM StockPrices SP "
        "INNER JOIN Company C ON C.CompanyId = SP.CompanyId AND C.CompanyName = '%s' "
        "WHERE SP.Time BETWEEN '%s' AND '%s' "
        , company_name, start_time, end_time);

}

char *GetStockPricesBetweenRangeDays(char *company_name, char *start_time, char *end_time) 
{

    return GetFormattedPointer(
        "SELECT SP.Price FROM StockPrices SP "
        "INNER JOIN Company C ON C.CompanyId = SP.CompanyId AND C.CompanyName = '%s' "
        "WHERE SP.Time BETWEEN '%s' AND '%s' "
        "AND (SP.StockPriceId - 1) %% 24 = 0 "
        , company_name, start_time, end_time);

}

char *GetStockPricesBetweenRangeWeekly(char *company_name, char *start_time, char *end_time) 
{

    return GetFormattedPointer(
        "SELECT SP.Price FROM StockPrices SP "
        "INNER JOIN Company C ON C.CompanyId = SP.CompanyId AND C.CompanyName = '%s' "
        "WHERE SP.Time BETWEEN '%s' AND '%s' "
        "AND (SP.StockPriceId - 1) %% 168 = 0 "
        , company_name, start_time, end_time);

}

char *GetStockPricesBetweenRangeMonthly(char *company_name, char *start_time, char *end_time) 
{

    return GetFormattedPointer(
        "DROP TABLE IF EXISTS TEMP; "
        "CREATE TABLE TEMP (Name VARCHAR(10), HourVal INTEGER, DayVal INTEGER); "
        "INSERT INTO TEMP (Name, HourVal, DayVal) VALUES ('After', strftime('%%H', '%s'), strftime('%%d', '%s')); "
        "SELECT SP.Price FROM StockPrices SP "
        "INNER JOIN Company C ON C.CompanyId = SP.CompanyId AND C.CompanyName = '%s' "
        "WHERE SP.Time BETWEEN '%s' AND '%s' "
        "AND strftime('%%H', SP.Time) = (SELECT HourVal FROM TEMP WHERE Name='After' LIMIT 1) "
        "AND strftime('%%d', SP.Time) = (SELECT DayVal FROM TEMP WHERE Name='After' LIMIT 1); "
        , end_time, end_time, company_name, start_time, end_time);

}

char *GetStockPricesBetweenRangeSixMonths(char *company_name, char *start_time, char *end_time) 
{

    return GetFormattedPointer(
        "DROP TABLE IF EXISTS TEMP; "
        "CREATE TABLE TEMP (Name VARCHAR(10), MonthVal INTEGER, HourVal INTEGER, DayVal INTEGER); "
        "INSERT INTO TEMP (Name, MonthVal, HourVal, DayVal) VALUES ('After',strftime('%%m', '%s'),  strftime('%%H', '%s'), strftime('%%d', '%s')); "
        "SELECT SP.Price FROM StockPrices SP "
        "INNER JOIN Company C ON C.CompanyId = SP.CompanyId AND C.CompanyName = '%s' "
        "WHERE SP.Time BETWEEN '%s' AND '%s' "
        "AND strftime('%%H', SP.Time) = (SELECT HourVal FROM TEMP WHERE Name='After' LIMIT 1) "
        "AND strftime('%%d', SP.Time) = (SELECT DayVal FROM TEMP WHERE Name='After' LIMIT 1) "
        "AND ABS(strftime('%%m', SP.Time) - (SELECT MonthVal FROM TEMP WHERE Name='After' LIMIT 1)) IN (0, 6); "
        , end_time, end_time, end_time, company_name, start_time, end_time);

}

char *GetStockPricesBetweenRangeQuery(char *company_name, char *start_time, char *end_time, TimeSpan timespan) 
{

    switch (timespan) {

        case ONE_DAY:
        case THREE_DAYS:   return GetStockPricesBetweenRangeHourly(company_name, start_time, end_time); break;
        case ONE_WEEK:
        case TWO_WEEKS:
        case ONE_MONTH:    return GetStockPricesBetweenRangeDays(company_name, start_time, end_time); break;
        case THREE_MONTHS:
        case SIX_MONTHS:
        case ONE_YEAR:     return GetStockPricesBetweenRangeWeekly(company_name, start_time, end_time); break;
        case TWO_YEARS:
        case FIVE_YEARS:   return GetStockPricesBetweenRangeMonthly(company_name, start_time, end_time); break;
        case ALL_TIME:     return GetStockPricesBetweenRangeSixMonths(company_name, start_time, end_time); break;

    }
    return NULL;

}

StockPrices *GetStockPricesBetweenRange(char *company_name, char *start_time, char *end_time, TimeSpan timespan) 
{

    StockPrices *prices = malloc(sizeof(StockPrices));
    prices->prices      = malloc(sizeof(float) * 128);
    prices->num_prices  = 0;
    prices->size        = 128;

    sqlite3 *db;
    if (OpenConnection(&db, MemoryConnection()) != 0)
        return prices;

    ExecuteQuery(GetStockPricesBetweenRangeQuery(company_name, start_time, end_time, timespan), &SetCompanyPrices, prices, db);

    sqlite3_close(db);

    prices->prices = realloc(prices->prices, sizeof(float) * prices->num_prices);
    return prices;

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