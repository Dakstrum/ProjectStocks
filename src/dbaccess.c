#include <stdio.h>
#include <stdbool.h>
#include <sqlite3.h>

#include "shared.h"

void SetupMainDB();
void SetupLogDB();

bool DoesCompanyExist(char *company_name, sqlite3 *db);
void SetCompanyToActive(char *company_name, sqlite3 *db);
void InsertNewCompany(char *company_name, double ipo, sqlite3 *db);


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

        char *drop_stocks = "DROP TABLE IF EXISTS Stocks;";
        sqlite3_exec(db, drop_stocks, NULL, 0, 0);

        char *setup =   "CREATE TABLE IF NOT EXISTS Company(CompanyId INTEGER PRIMARY KEY, Ipo DOUBLE NOT NULL, CompanyName VARCHAR(50) NOT NULL, IsActiveInJson INT DEFAULT 0);"
                        "CREATE TABLE IF NOT EXISTS CompanyMetadata(CompanyMetaId INTEGER PRIMARY KEY, CompanyId INT NOT NULL, TotalEmployees INT NOT NULL, Category VARCHAR(3) NOT NULL, StocksInMarket UNSIGNED BIG INT, SaveId INT NOT NULL);"
                        "CREATE TABLE IF NOT EXISTS Stocks(StockId INTEGER PRIMARY KEY, CompanyId INT NOT NULL, Price FLOAT NOT NULL, Time DATETIME NOT NULL);"
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

void InsertAndOrSetCompanyToActive(char *company_name, double ipo) 
{

    sqlite3 *db;
    if  (sqlite3_open_v2("blinky.db", &db, SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_READWRITE, NULL) == SQLITE_OK) {

        if (!DoesCompanyExist(company_name, db)) {

            

        }

    }
    sqlite3_close(db);

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

    char buffer[128];
    char *temp = GetFormattedBuffer(buffer, "SELECT CompanyId FROM Company WHERE CompanyName=%s;", company_name);
    sqlite3_exec(db, temp, &FindOutIfCompanyExists, &exists, 0);

    return exists;

}

void SetCompanyToActive(char *company_name, sqlite3 *db) 
{

    char buffer[128];
    char *temp = GetFormattedBuffer(buffer, "UPDATE Company SET IsActiveInJson=1 WHERE Company=%s;", company_name);
    sqlite3_exec(db, temp, NULL, 0, 0);

}

void InsertNewCompany(char *company_name, double ipo, sqlite3 *db) 
{

    char buffer[256];
    char *temp = GetFormattedBuffer(buffer, "INSER INTO Company(Ipo, CompanyName, IsActiveInJson) VALUES (%lf, %s, 1)", company_name, ipo);
    sqlite3_exec(db, temp, NULL, 0, 0);

}