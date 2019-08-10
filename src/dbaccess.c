#include <stdio.h>
#include <sqlite3.h>

void SetupMainDB();
void SetupLogDB();

void Log(const char *str) 
{

    sqlite3 *db;
    sqlite3_stmt *res;
    if (sqlite3_open("log.db", &db) == SQLITE_OK) {

        char *logger = "INSERT INTO LOGS (Log) VALUES (@log);";
        if (sqlite3_prepare_v2(db, logger, -1, &res, 0) == SQLITE_OK) {

            sqlite3_bind_text(res, sqlite3_bind_parameter_index(res, "@log"), str, -1, 0);
            sqlite3_step(res);

        }

    }
    sqlite3_finalize(res);
    sqlite3_close(db);

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
    if  (!sqlite3_open("log.db", &db)) {

        char *setup = "CREATE TABLE IF NOT EXISTS LOGS(LogId INTEGER PRIMARY KEY, TimeStamp DATETIME DEFAULT(datetime(CURRENT_TIMESTAMP, 'localtime')), Log TEXT NOT NULL);";
        sqlite3_exec(db, setup, NULL, 0, 0);

    }
    sqlite3_close(db);

}