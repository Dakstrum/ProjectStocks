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
#include "dbaccount.h"
#include "dbsave.h"
#include "math.h"


void SetupMainDB();
void SetupLogDB();

void InsertDefaultSettingsIfNeeded(sqlite3 *db);
int GetCompanyId(char *company_name);
bool DoesCompanyExist(char *company_name, sqlite3 *db);
void SetCompanyToActive(char *company_name, sqlite3 *db);
void InsertNewCompany(char *company_name, float ipo, sqlite3 *db);
int FindOutIfYouCanAddFromCurrentStock(void *owned_stock_amount, int argc, char **argv, char **col_name);
int FindOutIfYouCanSubtractFromCurrentStock(void *owned_stock_amount, int argc, char **argv, char **col_name);
TransactionType GetTransactionType(struct Transactions *transaction_temp);
float GetAmountPerShare(struct Transactions *transaction_temp);


int InsertAndOrSetCompanyToActive(char *company_name, float ipo) 
{

    int company_id = -1;

    sqlite3 *db;
    if  (OpenConnection(&db, DefaultConnection()) != 0)
        return -1;

    if (!DoesCompanyExist(company_name, db))
        InsertNewCompany(company_name, ipo, db);
    
    SetCompanyToActive(company_name, db);
    company_id = GetCompanyId(company_name);

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
    ExecuteQueryFDB(&FindOutIfCompanyExists, &exists, db, "SELECT CompanyId FROM Company WHERE CompanyName='%s';", company_name);

    return exists;

}

void SetCompanyToActive(char *company_name, sqlite3 *db) 
{

    ExecuteQueryFDB(NULL, NULL, db, "UPDATE Company SET IsActiveInJson=1 WHERE CompanyName='%s';", company_name);

}

void InsertNewCompany(char *company_name, float ipo, sqlite3 *db) 
{

    ExecuteQueryFDB(NULL, NULL, db, "INSERT INTO Company (Ipo, CompanyName, IsActiveInJson) VALUES (%f, '%s', 1);", ipo, company_name);

}

void AddOwnedStock(char *company_name, int amount_to_own) 
{

    ExecuteQueryF(NULL, NULL, "INSERT INTO OwnedStocks (SaveId, PlayerId, CompanyId, HowManyOwned) VALUES (%d, %d, %d, '%d');", GetSaveId(), GetCurrentPlayerId(),  GetCompanyId(company_name), amount_to_own);

}

int GetOwnedStockAmountCallback(void *owned_stock_amount, int argc, char **argv, char **col_name) 
{

    if (argc == 0)
        *((int *)owned_stock_amount) = -1;
    else
        *((int *)owned_stock_amount) = atoi(argv[0]);

    return 0;

}

int GetOwnedStockAmount(char *company_name) 
{

    int owned_stock_amount = 0;

    ExecuteQueryF(&GetOwnedStockAmountCallback, &owned_stock_amount, "SELECT HowManyOwned FROM OwnedStocks WHERE CompanyId = %d AND SaveId=%d;", GetCompanyId(company_name), GetSaveId());

    return owned_stock_amount;

}

void InsertStockTransaction(char *company_name, float transaction_amount, int stocks_exchanged) 
{

    ExecuteQueryF(NULL, NULL, "INSERT INTO Transactions (SaveId, PlayerId, CompanyId, TransactionAmount, StocksExchanged, TransactionTime) VALUES (%d, %d, %d, %.2f, %d, %d);", GetSaveId(), GetCurrentPlayerId(), GetCompanyId(company_name), transaction_amount, stocks_exchanged, GetGameTime());

}

void AttemptToAddFromCurrentStock(char *company_name, int amount_to_add, float price_per_stock)
{

    sqlite3 *db            = NULL;
    int owned_stock_amount = -1;

     if (OpenConnection(&db, DefaultConnection()) != 0)
        return;

    ExecuteQueryFDB(&FindOutIfYouCanAddFromCurrentStock, &owned_stock_amount, db, "SELECT HowManyOwned FROM OwnedStocks WHERE CompanyId=%d AND SaveId=%d AND PlayerId=%d;", GetCompanyId(company_name), GetSaveId(), GetCurrentPlayerId());
    if(owned_stock_amount <= -1) 
        AddOwnedStock(company_name, amount_to_add);
    else
        ExecuteQueryFDB(NULL, NULL, db, "UPDATE OwnedStocks SET HowManyOwned = HowManyOwned + %d WHERE CompanyId=%d AND SaveId=%d AND PlayerId=%d;", amount_to_add, GetCompanyId(company_name), GetSaveId(), GetCurrentPlayerId());
    
    InsertStockTransaction(company_name, -amount_to_add * price_per_stock, amount_to_add);

    sqlite3_close(db);

}

int FindOutIfYouCanAddFromCurrentStock(void *owned_stock_amount, int argc, char **argv, char **col_name)
{

    if (argc > 0) 
        *((int *)owned_stock_amount) = (int)atoi(argv[0]);
    
    return 0;

}

bool AttemptToSubtractFromCurrentStock(char *company_name, int amount_to_subtract, float price_per_stock)
{

    sqlite3 *db            = NULL;
    int owned_stock_amount = 0;
    bool successful        = false;
    
    if (OpenConnection(&db, DefaultConnection()) != 0)
        return false;

    ExecuteQueryFDB(&FindOutIfYouCanSubtractFromCurrentStock, &owned_stock_amount, db, "SELECT HowManyOwned FROM OwnedStocks WHERE CompanyId=%d AND SaveId=%d AND PlayerId=%d;", GetCompanyId(company_name), GetSaveId(), GetCurrentPlayerId());

    if (owned_stock_amount >= amount_to_subtract) {

        ExecuteQueryFDB(NULL, NULL, db, "UPDATE OwnedStocks SET HowManyOwned = HowManyOwned - %d  WHERE CompanyId=%d AND SaveId=%d AND PlayerId=%d;", amount_to_subtract, GetCompanyId(company_name), GetSaveId(), GetCurrentPlayerId());
        InsertStockTransaction(company_name, amount_to_subtract * price_per_stock, -amount_to_subtract);
        successful = true;

    }

    sqlite3_close(db);

    return successful;

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

int GetCompanyId(char *company_name) 
{

    int company_id = -1;

    ExecuteQueryF(&SetCompanyId, &company_id, "SELECT CompanyId FROM Company WHERE CompanyName='%s'", company_name);

    return company_id;

}

void InsertStockPrice(int save_id, int company_id, float stock_price, char *timestamp, sqlite3 *db) 
{

    ExecuteQueryFDB(NULL, NULL, db, "INSERT INTO StockPrices (SaveId, CompanyId, Price, Time) VALUES (%d, %d, %f, '%s')", save_id, company_id, stock_price, timestamp);

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

    ExecuteQueryF(&GetAmountOfSavesCallback, &amount_of_saves, "SELECT * FROM Saves");

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

    ExecuteQueryF(&GetStockNameFromStockIdCallback, &stock_name, "SELECT CompanyName FROM Company WHERE CompanyId = %d", stock_id);

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

    ExecuteQueryF(&GetAmountOfCompanysCallback, &amount_of_companies, "SELECT * FROM Company");

    return amount_of_companies;

}

int GetMoneyFromPlayersCallback(void *money, int argc, char **argv, char **col_name)
{

    if (argc > 0) 
       *((float *)money) = atof(argv[0]);
    
    return 0;

}

void SetLocalMoneyFromDb(int player_id)
{
    float money = 0.0;

    ExecuteQueryF(&GetMoneyFromPlayersCallback, &money, "SELECT Money FROM Players WHERE PlayerId = %d", player_id);

    SetAccountMoney(*((float *)&money));

}

void SetDBMoneyToLocalMoney(int player_id)
{

    ExecuteQueryF(NULL, NULL, "UPDATE Players SET Money = %.2f WHERE PlayerId = %d;", GetAccountMoney(), player_id);

}

int SetTransactionCallback(void *transaction, int argc, char **argv, char **col_name)
{
    
    struct Transactions *transaction_temp = (struct Transactions *)transaction;

    if (argc == 0 )
        return 0;

    if (transaction_temp->num_transactions == transaction_temp->size){

        transaction_temp->size        += 128;
        transaction_temp->transaction = realloc(transaction_temp->transaction, sizeof(float)  * transaction_temp->size);
        transaction_temp->date        = realloc(transaction_temp->date,        sizeof(time_t)  * transaction_temp->size);
        transaction_temp->shares      = realloc(transaction_temp->shares,      sizeof(int) * transaction_temp->size);
        transaction_temp->pershare    = realloc(transaction_temp->pershare,    sizeof(float) * transaction_temp->size);
        transaction_temp->type        = realloc(transaction_temp->type,        sizeof(TransactionType) * transaction_temp->size);

    }

    transaction_temp->transaction[transaction_temp->num_transactions] = (float)atof(argv[4]);
    transaction_temp->shares[transaction_temp->num_transactions]      = (int)atof(argv[5]);
    transaction_temp->date[transaction_temp->num_transactions]        = (time_t)atof(argv[6]);
    transaction_temp->type[transaction_temp->num_transactions]        = GetTransactionType(transaction_temp);
    transaction_temp->pershare[transaction_temp->num_transactions]    = GetAmountPerShare(transaction_temp);
    
    transaction_temp->num_transactions++;
    
    return 0;
}

TransactionType GetTransactionType(struct Transactions *transaction_temp)
{

    if(transaction_temp->transaction[transaction_temp->num_transactions] < 0)
        return BUY;
    else
        return SELL;

}

float GetAmountPerShare(struct Transactions *transaction_temp)
{

    return fabs(transaction_temp->transaction[transaction_temp->num_transactions] / (float)transaction_temp->shares[transaction_temp->num_transactions]);

}

struct Transactions *GetTransactions(char* company)
{

    struct Transactions *transaction      = malloc(sizeof(struct Transactions));
    transaction->transaction              = malloc(sizeof(float) * 128);
    transaction->shares                   = malloc(sizeof(int) * 128);
    transaction->pershare                 = malloc(sizeof(float) * 128);
    transaction->type                     = malloc(sizeof(TransactionType) * 128);
    transaction->date                     = malloc(sizeof(time_t) * 128);
    transaction->num_transactions         = 0;
    transaction->size                     = 128;

    for(int i = 0; i < 128; i++) {

        transaction->transaction[i] = 0;
        transaction->shares[i]      = 0;
        transaction->pershare[i]    = 0;
        transaction->date[i]        = 0;
        transaction->type[i]        = BUY;

    }

    ExecuteQueryF(&SetTransactionCallback, transaction, "SELECT * FROM Transactions WHERE CompanyId=%d AND SaveId=%d AND PlayerId=%d", GetCompanyId(company), GetSaveId(), GetCurrentPlayerId());
    return transaction;

}