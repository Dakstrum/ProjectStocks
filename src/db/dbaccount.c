#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sqlite3.h>

#include "queue.h"
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
static Transactions *account_transactions = NULL;
static Queue *transaction_queue   = NULL;

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

Transactions *GetNewTransactions() 
{

    Transactions *transactions     = malloc(sizeof(Transactions));
    transactions->id               = malloc(sizeof(int) * 128);
    transactions->company_id       = malloc(sizeof(int) * 128);
    transactions->transaction      = malloc(sizeof(float) * 128);
    transactions->shares           = malloc(sizeof(int) * 128);
    transactions->pershare         = malloc(sizeof(float) * 128);
    transactions->type             = malloc(sizeof(TransactionType) * 128);
    transactions->date             = malloc(sizeof(time_t) * 128);
    transactions->num_transactions = 0;
    transactions->size             = 128;

    for(int i = 0; i < 128; i++) {

        transactions->id[i]          = 0;
        transactions->company_id[i]  = 0;
        transactions->transaction[i] = 0;
        transactions->shares[i]      = 0;
        transactions->pershare[i]    = 0;
        transactions->date[i]        = 0;
        transactions->type[i]        = BUY;

    }

    return transactions;

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

int GetOwnedStockAmount(char *company_name) 
{

    const unsigned int company_id = GetCompanyId(company_name);

    for (size_t i = 0; i < num_companies;i++)
        if (owned_stocks.company_id[i] == company_id)
            return owned_stocks.owned_amount[i];


    return 0;

}

void ModifyOwnedStockAmount(char *company_name, int amount) 
{

    const unsigned int company_id = GetCompanyId(company_name);

    for (size_t i = 0; i < num_companies;i++) {

        if (owned_stocks.company_id[i] != company_id)
            continue;

        owned_stocks.owned_amount[i] -= amount;
        break;

    }

}

void InsertStockTransaction(char *company_name, float transaction_amount, int stocks_exchanged) 
{

    const unsigned int save_id    = GetSaveId();
    const unsigned int player_id  = GetCurrentPlayerId();
    const unsigned int company_id = GetCompanyId(company_name);
    const time_t game_time        = GetGameTime();
    static char *query            = "INSERT INTO Transactions (SaveId, PlayerId, CompanyId, TransactionAmount, StocksExchanged, TransactionTime) VALUES (%d, %d, %d, %.2f, %d, %d);";
    Queue_PushMessage(transaction_queue, GetFormattedPointer(query, save_id, player_id, company_id, transaction_amount, stocks_exchanged, game_time));

    IncreaseTransactionSizeIfNeeded(account_transactions);
    account_transactions->id[account_transactions->num_transactions]          = 0;
    account_transactions->company_id[account_transactions->num_transactions]  = company_id;
    account_transactions->transaction[account_transactions->num_transactions] = transaction_amount;
    account_transactions->shares[account_transactions->num_transactions]      = stocks_exchanged;
    account_transactions->date[account_transactions->num_transactions]        = game_time;
    account_transactions->type[account_transactions->num_transactions]        = GetTransactionType(account_transactions);
    account_transactions->pershare[account_transactions->num_transactions]    = GetAmountPerShare(account_transactions);
    account_transactions->num_transactions++;

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


void CopyTransactionEntry(Transactions *destination, Transactions *source, unsigned int source_idx)
{

    IncreaseTransactionSizeIfNeeded(destination);
    destination->id[destination->num_transactions]          = source->id[source_idx];
    destination->company_id[destination->num_transactions]  = source->company_id[source_idx];
    destination->transaction[destination->num_transactions] = source->transaction[source_idx];
    destination->shares[destination->num_transactions]      = source->shares[source_idx];
    destination->date[destination->num_transactions]        = source->date[source_idx];
    destination->type[destination->num_transactions]        = source->type[source_idx];
    destination->pershare[destination->num_transactions]    = source->pershare[source_idx];
    destination->num_transactions++;

}

Transactions *GetCompanyTransactions(char* company)
{

    Transactions *company_transactions = GetNewTransactions();
    const int company_id = GetCompanyId(company);

    for (size_t i = 0; i < account_transactions->num_transactions;i++) {

        if (account_transactions->company_id[i] != company_id)
            continue;

        CopyTransactionEntry(company_transactions, account_transactions, i);

    }
    return company_transactions;

}

Transactions *GetAllTransactions()
{

    Transactions *transactions = GetNewTransactions();
    for (size_t i = 0; i < account_transactions->num_transactions;i++)
        CopyTransactionEntry(transactions, account_transactions, i);

    return transactions;


}

struct Transactions *GetAllSavedTransactions()
{

    Transactions *transactions = GetNewTransactions();
    ExecuteQueryF(&SetTransactionCallback, transactions, "SELECT * FROM Transactions WHERE SaveId=%d AND PlayerId=%d", GetSaveId(), GetCurrentPlayerId());
    return transactions;

}

int GetOwnedStocks_CallBack(void *owned_amount, int argc, char **argv, char **col_name)
{

    if (argc == 0)
        return 0;

    unsigned int company_id = (unsigned int)atoi(argv[1]);
    for (size_t i = 0; i < num_companies;i++) {

        if (owned_stocks.company_id[i] != company_id)
            continue;

        owned_stocks.owned_amount[i] = (unsigned int)atoi(argv[0]);
        break;

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

        for (size_t i = 0; i< num_companies;i++)
            owned_stocks.company_id[i] = companies[i].company_id;

    }

    for (size_t i = 0; i< num_companies;i++)
        owned_stocks.owned_amount[i] = 0;

    ExecuteQueryF(&GetOwnedStocks_CallBack, NULL, "SELECT SUM(StocksExchanged), CompanyId FROM Transactions WHERE SaveId=%d AND PlayerId=%d GROUP BY CompanyId", GetSaveId(), GetCurrentPlayerId());

}

void SaveTransactions()
{

    Vector *vector = Queue_GetLockFreeVector(transaction_queue);

    sqlite3 *db;
    if (sqlite3_open_v2(DefaultConnection(), &db, SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK)
        return;

    ExecuteTransaction(db, vector);
    Vector_DeletePtrs(vector);

}

void FreeTransactions(Transactions *transactions)
{

    free(transactions->id);
    free(transactions->pershare);
    free(transactions->shares);
    free(transactions->transaction);
    free(transactions->date);
    free(transactions->type);
    free(transactions);

}

void InitializeTransactions()
{

    if (account_transactions != NULL) {

        FreeTransactions(account_transactions);
        account_transactions = NULL;

    }
    account_transactions = GetAllSavedTransactions();

}

void InitializeAccountInformation()
{


    InitializeOwnedStocks();
    InitializeTransactions();
    transaction_queue = Queue_Create();

}