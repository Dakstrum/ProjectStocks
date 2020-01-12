#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sqlite3.h>

#include "log.h"
#include "graph.h"
#include "shared.h"
#include "dbaccess.h"
#include "account.h"
#include "stocksmenu.h"

void SetupMainDB();
void SetupLogDB();

void InsertDefaultSettingsIfNeeded(sqlite3 *db);
int GetCompanyId(char *company_name, sqlite3 *db);
bool DoesCompanyExist(char *company_name, sqlite3 *db);
void SetCompanyToActive(char *company_name, sqlite3 *db);
void InsertNewCompany(char *company_name, float ipo, sqlite3 *db);
void ExecuteQuery(char *query, int (*callback)(void *,int, char**, char **), void *callback_var, sqlite3 *db);
int FindOutIfYouCanAddFromCurrentStock(void *owned_stock_amount, int argc, char **argv, char **col_name);
int FindOutIfYouCanSubtractFromCurrentStock(void *owned_stock_amount, int argc, char **argv, char **col_name);

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
                    "CREATE TABLE IF NOT EXISTS CompanyMetadata(CompanyMetaId INTEGER PRIMARY KEY, CompanyId INT NOT NULL, SaveId INT NOT NULL, TotalEmployees INT NOT NULL, StocksInMarket UNSIGNED BIG INT, PriceModifier DOUBLE DEFAULT(0.0), EntryTime DATETIME NOT NULL);"
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

void AddOwnedStock(char *company_name, int amount_to_own) 
{

    sqlite3 *db;

    if (OpenConnection(&db, DefaultConnection()) == 0) 
        ExecuteQuery(GetFormattedPointer("INSERT INTO OwnedStocks ( SaveId, PlayerName, CompanyId, HowManyOwned) VALUES (%d, 1, %d, '%d');", GetSaveId(), GetCompanyId(company_name, db), amount_to_own), NULL, NULL, db);

}

void InsertStockTransaction(char *company_name, int transaction_amount, int stocks_exchanged) 
{

    sqlite3 *db;

    if (OpenConnection(&db, DefaultConnection()) == 0)
        ExecuteQuery(GetFormattedPointer("INSERT INTO Transactions ( SaveId, PlayerName, CompanyId, TransactionAmount, StocksExchanged, TransactionTime) VALUES (%d, 1, %d, %d, %d, %d);", GetSaveId(), GetCompanyId(company_name, db), transaction_amount, stocks_exchanged, GetGameTime()), NULL, NULL, db);

}

void AttemptToAddFromCurrentStock(char *company_name, int amount_to_add, int price_per_stock)
{

    sqlite3 *db;
    int owned_stock_amount;

     if (OpenConnection(&db, DefaultConnection()) == 0) {

        ExecuteQuery(GetFormattedPointer("SELECT HowManyOwned FROM OwnedStocks WHERE CompanyId =%d;", GetCompanyId(company_name, db)), &FindOutIfYouCanAddFromCurrentStock, &owned_stock_amount, db);

        if(owned_stock_amount <= 0) 
            AddOwnedStock(company_name, amount_to_add);
        else
            ExecuteQuery(GetFormattedPointer("UPDATE OwnedStocks SET HowManyOwned = HowManyOwned + %d  WHERE CompanyId=%d;", amount_to_add, GetCompanyId(company_name, db)), NULL, NULL, db);
        
        InsertStockTransaction(company_name, -amount_to_add * price_per_stock, amount_to_add);

    }

}

int FindOutIfYouCanAddFromCurrentStock(void *owned_stock_amount, int argc, char **argv, char **col_name)
{

    if (argc > 0)
        *((int *)owned_stock_amount) = (int)atoi(argv[0]);
    
    return 0;

}

void AttemptToSubtractFromCurrentStock(char *company_name, int amount_to_subtract, int price_per_stock)
{

    sqlite3 *db;
    int owned_stock_amount;
    
    if (OpenConnection(&db, DefaultConnection()) == 0) {

        ExecuteQuery(GetFormattedPointer("SELECT HowManyOwned FROM OwnedStocks WHERE CompanyId=%d;", GetCompanyId(company_name, db)), &FindOutIfYouCanSubtractFromCurrentStock, &owned_stock_amount, db);

        if (owned_stock_amount >= amount_to_subtract) {

            ExecuteQuery(GetFormattedPointer("UPDATE OwnedStocks SET HowManyOwned = HowManyOwned - %d  WHERE CompanyId=%d;", amount_to_subtract, GetCompanyId(company_name, db)), NULL, NULL, db);
            InsertStockTransaction(company_name, amount_to_subtract * price_per_stock, -amount_to_subtract);
            
        }

    }

}

int FindOutIfYouCanSubtractFromCurrentStock(void *owned_stock_amount, int argc, char **argv, char **col_name)
{

    if (argc > 0)
        *((int *)owned_stock_amount) = (int)atoi(argv[0]);
    
    return 0;
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

int GetSaveNameFromSaveIdCallback(void *save_name, int argc, char **argv, char **col_name)
{

    if (argc > 0) {

        char *temp = *((char **)save_name);
        strncpy(temp, argv[0], 127);
        temp[127] = '\0';

    }

    return 0;

}

char *GetSaveNameFromSaveId(int save_id)
{

    char *save_name = malloc(sizeof(char) * 128);

    sqlite3 *db;
    if (OpenConnection(&db, DefaultConnection()) == 0)
        ExecuteQuery(GetFormattedPointer("SELECT SaveName FROM Saves WHERE SaveId = %d", save_id), &GetSaveNameFromSaveIdCallback, &save_name, db);

    return save_name;

}

int GetPlayerNameFromSaveNameCallback(void *player_name, int argc, char **argv, char **col_name)
{

    if (argc > 0) {

        char *temp = *((char **)player_name);
        strncpy(temp, argv[0], 127);
        temp[127] = '\0';

    }

    return 0;

}

char *GetPlayerNameFromSaveName(char *save_name)
{

    char *player_name = malloc(sizeof(char) * 128);

    sqlite3 *db;
    if (OpenConnection(&db, DefaultConnection()) == 0)
        ExecuteQuery(GetFormattedPointer("SELECT PlayerName FROM Saves WHERE SaveName = '%s'", save_name), &GetPlayerNameFromSaveNameCallback, &player_name, db);

    return player_name;

}

int GetAmountOfSavesCallback(void *amount_of_saves, int argc, char **argv, char **col_name)
{

    if (argc > 0) 
        *((int *)amount_of_saves) = atoi(argv[0]);

    return 0;
}

int GetAmountOfSaves()
{

    int amount_of_saves;

    sqlite3 *db;
    if (OpenConnection(&db, DefaultConnection()) == 0)
        ExecuteQuery(GetFormattedPointer("SELECT * FROM Saves"), &GetAmountOfSavesCallback, &amount_of_saves, db);

    return amount_of_saves;

}

int GetStockNameFromStockIdCallback(void *stock_name, int argc, char **argv, char **col_name)
{

    if (argc > 0) {

        char *temp = *((char **)stock_name);
        strncpy(temp, argv[0], 127);
        temp[127] = '\0';

    }

    return 0;
}

char *GetStockNameFromStockId(int stock_id)
{
    char *stock_name = malloc(sizeof(char) * 128);

    sqlite3 *db;
    if (OpenConnection(&db, DefaultConnection()) == 0)
        ExecuteQuery(GetFormattedPointer("SELECT CompanyName FROM Company WHERE CompanyId = %d", stock_id), &GetStockNameFromStockIdCallback, &stock_name, db);

    return stock_name;
}

int GetAmountOfCompanysCallback(void *amount_of_saves, int argc, char **argv, char **col_name)
{

    if (argc > 0) 
        *((int *)amount_of_saves) = atoi(argv[0]);

    return 0;
}

int GetAmountOfCompanies()
{

    int amount_of_companies;

    sqlite3 *db;
    if (OpenConnection(&db, DefaultConnection()) == 0)
        ExecuteQuery(GetFormattedPointer("SELECT * FROM Company"), &GetAmountOfCompanysCallback, &amount_of_companies, db);

    return amount_of_companies;

}

void ExecuteQuery(char *query, int (*callback)(void *,int, char**, char **), void *callback_var, sqlite3 *db) 
{

    char *error = NULL;
    sqlite3_exec(db, query, callback, callback_var, &error);

    if (error != NULL)
        LogF("SQL ERROR %s, query = %s", error, query);

    free(query);

}
