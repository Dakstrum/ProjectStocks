#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sqlite3.h>

#include "account.h"
#include "shared.h"
#include "dbutils.h"
#include "dbcompany.h"
#include "dbaccount.h"
#include "dbsave.h"

typedef struct OwnedStocks
{

    unsigned int *company_id;
    unsigned int *owned_amount;

} OwnedStocks;

static unsigned int num_companies = 0;
static OwnedStocks owned_stocks   = {NULL, NULL};
static Transactions *transactions = NULL;

int GetOwnedStockAmount(char *company_name) 
{

    unsigned int company_id = GetCompanyId(company_name);

    for (unsigned int i = 0; i < num_companies;i++) {

        if (owned_stocks.company_id[i] == company_id)
            return owned_stocks.owned_amount[i];

    }

    return 0;

}

void ModifyOwnedStockAmount(char *company_name, int amount) 
{

    unsigned int company_id = GetCompanyId(company_name);

    for (unsigned int i = 0; i < num_companies;i++) {

        if (owned_stocks.company_id[i] == company_id) {

            owned_stocks.owned_amount[i] -= amount;
            break;

        }

    }

}

void InsertStockTransaction(char *company_name, float transaction_amount, int stocks_exchanged) 
{

    ExecuteQueryF(NULL, NULL, "INSERT INTO Transactions (SaveId, PlayerId, CompanyId, TransactionAmount, StocksExchanged, TransactionTime) VALUES (%d, %d, %d, %.2f, %d, %d);", GetSaveId(), GetCurrentPlayerId(), GetCompanyId(company_name), transaction_amount, stocks_exchanged, GetGameTime());

}


void AttemptToAddFromCurrentStock(char *company_name, int amount_to_add, float price_per_stock)
{

    ModifyOwnedStockAmount(company_name, amount_to_add);
    InsertStockTransaction(company_name, -amount_to_add * price_per_stock, amount_to_add);

}

bool AttemptToSubtractFromCurrentStock(char *company_name, int amount_to_subtract, float price_per_stock)
{

    if (GetOwnedStockAmount(company_name) < amount_to_subtract)
        return false;

    ModifyOwnedStockAmount(company_name, amount_to_subtract);
    InsertStockTransaction(company_name, amount_to_subtract * price_per_stock, -amount_to_subtract);
    return true;

}

TransactionType GetTransactionType(Transactions *transaction_temp)
{

    if(transaction_temp->transaction[transaction_temp->num_transactions] < 0)
        return BUY;
    else
        return SELL;

}

float GetAmountPerShare(Transactions *transaction_temp)
{

    return fabs(transaction_temp->transaction[transaction_temp->num_transactions] / (float)transaction_temp->shares[transaction_temp->num_transactions]);

}

void IncreaseTransactionSizeIfNeeded(Transactions *transaction_temp)
{

    if (transaction_temp->num_transactions < transaction_temp->size)
        return;

    transaction_temp->size        += 128;
    transaction_temp->id          = realloc(transaction_temp->id,          sizeof(int)  * transaction_temp->size);
    transaction_temp->company_id  = realloc(transaction_temp->company_id,  sizeof(int)  * transaction_temp->size);
    transaction_temp->transaction = realloc(transaction_temp->transaction, sizeof(float)  * transaction_temp->size);
    transaction_temp->date        = realloc(transaction_temp->date,        sizeof(time_t)  * transaction_temp->size);
    transaction_temp->shares      = realloc(transaction_temp->shares,      sizeof(int) * transaction_temp->size);
    transaction_temp->pershare    = realloc(transaction_temp->pershare,    sizeof(float) * transaction_temp->size);
    transaction_temp->type        = realloc(transaction_temp->type,        sizeof(TransactionType) * transaction_temp->size);


}

int SetTransactionCallback(void *transaction, int argc, char **argv, char **col_name)
{

    if (argc == 0)
        return 0;

    Transactions *transaction_temp  = (Transactions *)transaction;
    const int temp_num_transactions = transaction_temp->num_transactions;
    IncreaseTransactionSizeIfNeeded(transaction_temp);

    transaction_temp->id[temp_num_transactions]          = atoi(argv[0]);
    transaction_temp->company_id[temp_num_transactions]  = atoi(argv[3]);
    transaction_temp->transaction[temp_num_transactions] = (float)atof(argv[4]);
    transaction_temp->shares[temp_num_transactions]      = atoi(argv[5]);
    transaction_temp->date[temp_num_transactions]        = atoi(argv[6]);
    transaction_temp->type[temp_num_transactions]        = GetTransactionType(transaction_temp);
    transaction_temp->pershare[temp_num_transactions]    = GetAmountPerShare(transaction_temp);
    transaction_temp->num_transactions++;
    
    return 0;
}

struct Transactions *GetTransactions(char* company)
{

    Transactions *transaction      = malloc(sizeof(struct Transactions));
    transaction->id                = malloc(sizeof(int) * 128);
    transaction->company_id        = malloc(sizeof(int) * 128);
    transaction->transaction       = malloc(sizeof(float) * 128);
    transaction->shares            = malloc(sizeof(int) * 128);
    transaction->pershare          = malloc(sizeof(float) * 128);
    transaction->type              = malloc(sizeof(TransactionType) * 128);
    transaction->date              = malloc(sizeof(time_t) * 128);
    transaction->num_transactions  = 0;
    transaction->size              = 128;

    for(int i = 0; i < 128; i++) {

        transaction->id[i]          = 0;
        transaction->company_id[i]  = 0;
        transaction->transaction[i] = 0;
        transaction->shares[i]      = 0;
        transaction->pershare[i]    = 0;
        transaction->date[i]        = 0;
        transaction->type[i]        = BUY;

    }

    ExecuteQueryF(&SetTransactionCallback, transaction, "SELECT * FROM Transactions WHERE CompanyId=%d AND SaveId=%d AND PlayerId=%d", GetCompanyId(company), GetSaveId(), GetCurrentPlayerId());
    return transaction;

}

struct Transactions *GetAllTransactions()
{

    Transactions *transaction      = malloc(sizeof(struct Transactions));
    transaction->id                = malloc(sizeof(int) * 128);
    transaction->company_id        = malloc(sizeof(int) * 128);
    transaction->transaction       = malloc(sizeof(float) * 128);
    transaction->shares            = malloc(sizeof(int) * 128);
    transaction->pershare          = malloc(sizeof(float) * 128);
    transaction->type              = malloc(sizeof(TransactionType) * 128);
    transaction->date              = malloc(sizeof(time_t) * 128);
    transaction->num_transactions  = 0;
    transaction->size              = 128;

    for(int i = 0; i < 128; i++) {

        transaction->id[i]          = 0;
        transaction->company_id[i]  = 0;
        transaction->transaction[i] = 0;
        transaction->shares[i]      = 0;
        transaction->pershare[i]    = 0;
        transaction->date[i]        = 0;
        transaction->type[i]        = BUY;

    }

    ExecuteQueryF(&SetTransactionCallback, transaction, "SELECT * FROM Transactions WHERE SaveId=%d AND PlayerId=%d", GetSaveId(), GetCurrentPlayerId());
    return transaction;

}

int GetOwnedStocks_CallBack(void *owned_amount, int argc, char **argv, char **col_name)
{

    if (argc == 0)
        return 0;

    unsigned int company_id = (unsigned int)atoi(argv[1]);
    for (unsigned int i = 0; i < num_companies;i++) {

        if (owned_stocks.company_id[i] == company_id) {

            owned_stocks.owned_amount[i] = (unsigned int)atoi(argv[0]);
            break;

        }

    }
    return 0;

}

void InitializeOwnedStocks()
{

    Company *companies = GetAllCompanies();
    if (owned_stocks.company_id == NULL) {

        num_companies = GetNumCompanies();
        owned_stocks.company_id   = malloc(sizeof(int) * num_companies);
        owned_stocks.owned_amount = malloc(sizeof(int) * num_companies);

        for (unsigned int i = 0; i< num_companies;i++)
            owned_stocks.company_id[i] = companies[i].company_id;

    }

    for (unsigned int i = 0; i< num_companies;i++)
        owned_stocks.owned_amount[i] = 0;

    ExecuteQueryF(&GetOwnedStocks_CallBack, NULL, "SELECT SUM(StocksExchanged), CompanyId FROM Transactions WHERE SaveId=%d AND PlayerId=%d GROUP BY CompanyId", GetSaveId(), GetCurrentPlayerId());

}

void InitializeTransactions()
{

    transactions = GetAllTransactions();

}

void InitializeAccountInformation()
{


    InitializeOwnedStocks();
    InitializeTransactions();


}