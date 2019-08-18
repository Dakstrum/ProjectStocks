#include <stdio.h>
#include <sqlite3.h>

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

void SetupMainDB() 
{

    sqlite3 *db;
    if  (sqlite3_open_v2("blinky.db", &db, SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL) == SQLITE_OK) {

        char *setup =   "CREATE TABLE IF NOT EXISTS COMPANIES(CompanyId INTEGER PRIMARY KEY, CompanyName TEXT NOT NULL, TotalEmployees INT NOT NULL, Category VARCHAR(3) NOT NULL, StocksInMarket UNSIGNED BIG INT, SaveId INT NOT NULL);"
                        "CREATE TABLE IF NOT EXISTS STOCKS(StockId INTEGER PRIMARY KEY, CompanyId INT NOT NULL, Price FLOAT NOT NULL, Time DATETIME NOT NULL);"
                        "CREATE TABLE IF NOT EXISTS SAVES(SaveId INTEGER PRIMARY KEY, SaveName TEXT NOT NULL, Money DOUBLE NOT NULL, TimeSpentInGame UNSIGNED BIG INT, RandomSeed UNSIGNED BIG INT);"
                        "CREATE TABLE IF NOT EXISTS OWNEDSTOCKS(OwnedStockId INTEGER PRIMARY KEY, SaveId INT NOT NULL, CompanyId INT NOT NULL, HowManyOwned UNSIGNED BIG INT NOT NULL);"
                        "CREATE TABLE IF NOT EXISTS TRANSACTIONS(TransactionId INTEGER PRIMARY KEY, CompanyId INT NOT NULL, TransactionAmount DOUBLE NOT NULL, StocksExchanged INT NOT NULL, TransactionTime DATETIME NOT NULL, SaveId INT NOT NULL)";
        sqlite3_exec(db, setup, NULL, 0, 0);

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