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

int OpenConnection(sqlite3 **db) 
{

    if (sqlite3_open_v2("blinky.db", db, SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {

        Log("Could not establish connection to blinky.db from simulation");
        SetCleanUpToTrue();
        return -1;

    }
    return 0;

}

void SetupMainDB() 
{

    sqlite3 *db;
    if  (sqlite3_open_v2("blinky.db", &db, SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL) == SQLITE_OK) {

        char *drop_stocks = "DROP TABLE IF EXISTS StockPrices;";
        sqlite3_exec(db, drop_stocks, NULL, 0, 0);

        char *setup =   "CREATE TABLE IF NOT EXISTS Company(CompanyId INTEGER PRIMARY KEY, Ipo DOUBLE NOT NULL, CompanyName VARCHAR(50) NOT NULL, Category VARCHAR(30), IsActiveInJson INT DEFAULT 0);"
                        "CREATE TABLE IF NOT EXISTS CompanyMetadata(CompanyMetaId INTEGER PRIMARY KEY, CompanyId INT NOT NULL, TotalEmployees INT NOT NULL, StocksInMarket UNSIGNED BIG INT, SaveId INT NOT NULL);"
                        "CREATE TABLE IF NOT EXISTS StockPrices(StockPriceId INTEGER PRIMARY KEY, SaveId INT NOT NULL,  CompanyId INT NOT NULL, Price FLOAT NOT NULL, Time DATETIME NOT NULL);"
                        "CREATE TABLE IF NOT EXISTS Saves(SaveId INTEGER PRIMARY KEY, SaveName TEXT NOT NULL, Money DOUBLE NOT NULL, TimeSpentInGame UNSIGNED BIG INT, RandomSeed UNSIGNED BIG INT);"
                        "CREATE TABLE IF NOT EXISTS OwnedStocks(OwnedStockId INTEGER PRIMARY KEY, SaveId INT NOT NULL, CompanyId INT NOT NULL, HowManyOwned UNSIGNED BIG INT NOT NULL);"
                        "CREATE TABLE IF NOT EXISTS Transactions(TransactionId INTEGER PRIMARY KEY, CompanyId INT NOT NULL, TransactionAmount DOUBLE NOT NULL, StocksExchanged INT NOT NULL, TransactionTime DATETIME NOT NULL, SaveId INT NOT NULL)";
        sqlite3_exec(db, setup, NULL, 0, 0);

        char *set_all_companies_to_inactive = "UPDATE Company SET IsActiveInJson=0;";
        sqlite3_exec(db, set_all_companies_to_inactive, NULL, 0, 0);

    }
    sqlite3_close(db);

}

void SetupLogDB() 
{

    sqlite3 *db;
    if  (sqlite3_open_v2("log.db", &db, SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL) == SQLITE_OK) {

        char *setup = "CREATE TABLE IF NOT EXISTS LOGS(LogId INTEGER PRIMARY KEY, TimeStamp DATETIME DEFAULT(datetime(CURRENT_TIMESTAMP, 'localtime')), Log TEXT NOT NULL);";
        sqlite3_exec(db, setup, NULL, 0, 0);

    }
    sqlite3_close(db);

}

int InsertAndOrSetCompanyToActive(char *company_name, float ipo) 
{

    int company_id = -1;

    sqlite3 *db;
    if  (sqlite3_open_v2("blinky.db", &db, SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_READWRITE, NULL) == SQLITE_OK) {

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

/*

GetFormattedPointer("SELECT SP.Price FROM StockPrices SP" 
                        " INNER JOIN Company C ON C.CompanyId=SP.CompanyId AND C.CompanyName='%s' "
                        " WHERE SP.Time BETWEEN '%s' AND '%s' ", company_name, start_time, end_time)

*/

StockPrices *GetStockPricesBetweenRange(char *company_name, char *start_time, char *end_time, TimeSpan timespan) 
{

    StockPrices *prices = malloc(sizeof(StockPrices));
    prices->prices      = malloc(sizeof(float) * 128);
    prices->num_prices  = 0;
    prices->size        = 128;

    sqlite3 *db;
    if (OpenConnection(&db) != 0)
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