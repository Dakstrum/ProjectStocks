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
int FindOutIfYouCanAddFromCurrentStock(void *owned_stock_amount, int argc, char **argv, char **col_name);
int FindOutIfYouCanSubtractFromCurrentStock(void *owned_stock_amount, int argc, char **argv, char **col_name);


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

    player_name = '\0';
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

    int amount_of_saves = 0;

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

    int amount_of_companies = 0;

    sqlite3 *db;
    if (OpenConnection(&db, DefaultConnection()) == 0)
        ExecuteQuery(GetFormattedPointer("SELECT * FROM Company"), &GetAmountOfCompanysCallback, &amount_of_companies, db);

    return amount_of_companies;

}