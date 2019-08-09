#include <stdio.h>
#include <sqlite3.h>

void SetupMainDB();
void SetupLogDB();

void Log(const char *format, ...) 
{

    va_list args;
    va_start(args, format);
    //vfprintf(fp, format, args);
    va_end(args);

}

void InitializeDatabases() 
{

    SetupMainDB();
    SetupLogDB();
    
}

void SetupMainDB() 
{

    sqlite3 *db;
    if  (!sqlite3_open("blinky.db", &db)) {

        char *setup =   "CREATE TABLE IF NOT EXISTS COMPANIES(CompanyId INT PRIMARY KEY NOT NULL, CompanyName TEXT NOT NULL, TotalEmployees INT NOT NULL, Category VARCHAR(3) NOT NULL, StocksInMarket UNSIGNED BIG INT, SaveId INT NOT NULL);"
                        "CREATE TABLE IF NOT EXISTS STOCKS(StockId INT PRIMARY KEY NOT NULL, CompanyId INT NOT NULL, Price FLOAT NOT NULL, Time DATETIME NOT NULL);"
                        "CREATE TABLE IF NOT EXISTS SAVES(SaveId INT PRIMARY KEY NOT NULL, SaveName TEXT NOT NULL, Money DOUBLE NOT NULL, TimeSpentInGame UNSIGNED BIG INT, RandomSeed UNSIGNED BIG INT);"
                        "CREATE TABLE IF NOT EXISTS OWNEDSTOCKS(OwnedStockId INT PRIMARY KEY NOT NULL, SaveId INT NOT NULL, CompanyId INT NOT NULL, HowManyOwned UNSIGNED BIG INT NOT NULL);"
                        "CREATE TABLE IF NOT EXISTS TRANSACTIONS(TransactionId INT PRIMARY KEY NOT NULL, CompanyId INT NOT NULL, TransactionAmount DOUBLE NOT NULL, StocksExchanged INT NOT NULL, TransactionTime DATETIME NOT NULL, SaveId INT NOT NULL)";
        sqlite3_exec(db, setup, NULL, 0, 0);

    }

}

void SetupLogDB() 
{

    sqlite3 *db;
    if  (!sqlite3_open("log.db", &db)) {

        char *setup = "CREATE TABLE IF NOT EXISTS LOGS(LogId INT PRIMARY KEY NOT NULL, TimeStampt DATETIME NOT NULL, Log TEXT NOT NULL);";
        sqlite3_exec(db, setup, NULL, 0, 0);

    }

}