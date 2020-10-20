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
#include "log.h"
#include "simulation.h"
#include "vector.h"

typedef struct OwnedStocks
{

    unsigned int *company_id;
    unsigned int *owned_amount;

} OwnedStocks;

static unsigned int num_companies = 0;
static OwnedStocks owned_stocks   = {NULL, NULL};
static Queue *transaction_queue   = NULL;

static Vector *transactions = NULL;
static Vector *company_ids  = NULL;
static Vector *player_ids   = NULL;

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

        owned_stocks.owned_amount[i] += amount;
        break;

    }

}

void InsertStockTransaction(unsigned int player_id, char *company_name, float transaction_amount, int stocks_exchanged) 
{

    const unsigned int save_id    = GetSaveId();
    const unsigned int company_id = GetCompanyId(company_name);
    const time_t game_time        = GetGameTime();
    static char *query            = "INSERT INTO Player_Transactions (PlayerId, CompanyId, TransactionAmount, StocksExchanged, TransactionTime) VALUES (%d, %d, %.2f, %d, %d);";
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


void AttemptToAddFromCurrentStock(unsigned int player_id, char *company_name, int amount_to_add, float price_per_stock)
{

    ModifyOwnedStockAmount(company_name, amount_to_add);
    InsertStockTransaction(company_name, -amount_to_add * price_per_stock, amount_to_add);

}

bool AttemptToSubtractFromCurrentStock(unsigned int player_id, char *company_name, int amount_to_subtract, float price_per_stock)
{

    if (GetOwnedStockAmount(company_name) < amount_to_subtract)
        return false;

    ModifyOwnedStockAmount(company_name, -amount_to_subtract);
    InsertStockTransaction(company_name, amount_to_subtract * price_per_stock, -amount_to_subtract);
    return true;

}

int SetTransactionCallback(void *transaction, int argc, char **argv, char **col_name)
{

    if (argc == 0)
        return 0;

    int player_id  = atoi(argv[1]);
    int company_id = atoi(argv[2]);

    Vector_PushBack(company_ids, &company_id);
    Vector_PushBack(player_ids, &player_id);

    Transaction transaction;
    transaction.transaction_id     = atoi(argv[0]);
    transaction.transaction_amount = atof(argv[3]);
    transaction.shares_exhanged    = atoi(argv[4]);
    transaction.transaction_date   = atoi(argv[5]);
    transaction.price_per_share    = transaction.transaction_amount / transaction.shares_exhanged;
    transaction.type = transaction.transaction_amount < 0 ? BUY : SELL;

    Vector_PushBack(transactions, &transaction);
    
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

void InitSavedTransactions()
{

    char *query =   "SELECT PT.* FROM Player_Transactions PT "
                    "INNER JOIN Game_Players GP ON GP.PlayerId = PT.PlayerId "
                    "WHERE GP.SaveId = %d";

    ExecuteQueryF(&SetTransactionCallback, NULL, query, GetSaveId());

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

    ExecuteQueryF(&GetOwnedStocks_CallBack, NULL, "SELECT SUM(StocksExchanged), CompanyId FROM Player_Transactions WHERE PlayerId=%d GROUP BY CompanyId", GetCurrentPlayerId());

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

void InitializeAccountInformation()
{

    if (transactions == NULL) {

        transactions = Vector_Create(sizeof(Transaction), 32);
        company_ids  = Vector_Create(sizeof(int), 32);
        player_ids   = Vector_Create(sizeof(int), 32); 

    } else {

        Vector_Reset(transactions);
        Vector_Reset(company_ids);
        Vector_Reset(player_ids);

    }

    InitializeOwnedStocks();
    InitSavedTransactions();
    transaction_queue = Queue_Create();

}

float GetAccountNetWorth()
{

    float networth = 0;
    for(int i = 1; i < GetNumCompanies(); i++) {

        float networth_of_specific_stock = GetOwnedStockAmount(GetCompanyName(i)) * CurrentStockPrice(GetCompanyName(i));
        networth = networth + networth_of_specific_stock;

    }
    return networth;

}