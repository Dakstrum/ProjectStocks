#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sqlite3.h>

#include "queue.h"
#include "account.h"
#include "game.h"
#include "shared.h"
#include "dbutils.h"
#include "dbcompany.h"
#include "dbaccount.h"
#include "dbsave.h"
#include "log.h"
#include "simulation.h"
#include "vector.h"


static unsigned int num_companies = 0;
static Queue *transaction_queue   = NULL;

static Vector *save_player_ids = NULL;

static Vector *transactions = NULL;
static Vector *transaction_company_ids = NULL;
static Vector *transaction_player_ids  = NULL;

static Vector *owned_stocks = NULL;
static Vector *owned_company_ids = NULL;
static Vector *owned_player_ids  = NULL;


TransactionType GetTransactionType(float transaction_amount)
{

    return transaction_amount < 0 ? BUY : SELL;

}

float GetPricePerShare(int shares_exchanged, float transaction_amount)
{

    return fabs(transaction_amount / shares_exchanged);

}

uint32_t dbaccount_get_owned_stock_amount(uint32_t player_id, uint32_t company_id)
{

    uint32_t *company_ids = owned_company_ids->elements;
    uint32_t *player_ids  = owned_player_ids->elements;
    uint32_t *owned_stock_amounts = owned_stocks->elements;
    for (size_t i = 0; i < owned_stocks->num_elements;i++) {

        if (company_ids[i] == company_id && player_ids[i] == player_id) {

            return owned_stock_amounts[i];

        }

    }

    return 0;

}

void ModifyOwnedStockAmount(uint32_t player_id, uint32_t company_id, int amount) 
{

    uint32_t *company_ids = owned_company_ids->elements;
    uint32_t *player_ids  = owned_player_ids->elements;
    uint32_t *owned_stock_amounts = owned_stocks->elements;
    for (size_t i = 0; i < owned_stocks->num_elements;i++) {

        if (company_ids[i] == company_id && player_ids[i] == player_id) {

            owned_stock_amounts[i] += amount;
            break;

        }

    }

}

void dbaccount_insert_transaction(unsigned int player_id, uint32_t company_id, float transaction_amount, int stocks_exchanged) 
{

    time_t game_time   = Game_GetGameTime();
    static char *query = "INSERT INTO Player_Transactions (PlayerId, CompanyId, TransactionAmount, StocksExchanged, TransactionTime) VALUES (%d, %d, %.2f, %d, %d);";
    Queue_PushMessage(transaction_queue, GetFormattedPointer(query, player_id, company_id, transaction_amount, stocks_exchanged, game_time));

    Vector_PushBack(transaction_player_ids, &player_id);
    Vector_PushBack(transaction_company_ids, &company_id);

    Transaction transaction = {0, company_id, game_time, stocks_exchanged, GetPricePerShare(stocks_exchanged, transaction_amount), transaction_amount, GetTransactionType(transaction_amount)};

    Vector_PushBack(transactions, &transaction);

}


void dbaccount_buy_stocks(uint32_t player_id, uint32_t company_id, int amount_to_add, float price_per_stock)
{

    ModifyOwnedStockAmount(player_id, company_id, amount_to_add);
    dbaccount_insert_transaction(player_id, company_id, -amount_to_add * price_per_stock, amount_to_add);

}

bool dbaccount_can_sell_stock(uint32_t player_id, uint32_t company_id, uint32_t amount_to_subtract)
{

    return dbaccount_get_owned_stock_amount(player_id, company_id) >= amount_to_subtract;

}

bool dbaccount_sell_stocks(uint32_t player_id, uint32_t company_id, int amount_to_subtract, float price_per_stock)
{

    ModifyOwnedStockAmount(player_id, company_id, -amount_to_subtract);
    dbaccount_insert_transaction(player_id, company_id, amount_to_subtract * price_per_stock, -amount_to_subtract);
    return true;

}

int SetTransactionCallback(void *transaction, int argc, char **argv, char **col_name)
{

    if (argc == 0)
        return 0;

    uint32_t player_id  = (uint32_t)atoi(argv[1]);
    uint32_t company_id = (uint32_t)atoi(argv[2]);

    Vector_PushBack(transaction_company_ids, &company_id);
    Vector_PushBack(transaction_player_ids, &player_id);

    Transaction temp;
    temp.transaction_id     = atoi(argv[0]);
    temp.company_id         = company_id;
    temp.transaction_amount = atof(argv[3]);
    temp.shares_exchanged   = atoi(argv[4]);
    temp.transaction_date   = atoi(argv[5]);
    temp.price_per_share    = temp.transaction_amount / temp.shares_exchanged;
    temp.type = temp.transaction_amount < 0 ? BUY : SELL;

    Vector_PushBack(transactions, &temp);
    
    return 0;

}

Vector *dbaccount_get_company_transactions(uint32_t player_id, uint32_t company_id)
{

    uint32_t *company_ids = transaction_company_ids->elements;
    uint32_t *player_ids = transaction_player_ids->elements;
    Transaction *company_transactions = transactions->elements;

    Vector *temp_transactions = Vector_Create(sizeof(Transaction), 16);

    for (size_t i = 0; i < transactions->num_elements;i++) {

        if (company_ids[i] == company_id && player_ids[i] == player_id) {

            Vector_PushBack(temp_transactions, &company_transactions[i]);

        }

    }

    return temp_transactions;

}

Vector *dbaccount_get_player_transactions(uint32_t player_id)
{

    uint32_t *player_ids = transaction_player_ids->elements;
    Transaction *company_transactions = transactions->elements;

    Vector *temp_transactions = Vector_Create(sizeof(Transaction), 16);

    for (size_t i = 0; i < transactions->num_elements;i++) {

        if (player_ids[i] == player_id) {

            Vector_PushBack(temp_transactions, &company_transactions[i]);

        }

    }

    return temp_transactions;

}

Vector *dbaccount_get_all_transactions()
{

    return transactions;

}

void InitSavedTransactions()
{

    char *query = 
    "SELECT PT.* FROM Player_Transactions PT "
    "INNER JOIN Game_Players GP ON GP.PlayerId = PT.PlayerId "
    "WHERE GP.SaveId = %d";

    ExecuteQueryF(&SetTransactionCallback, NULL, query, Game_GetSaveId());

}

int GetOwnedStocks_CallBack(void *owned_amount, int argc, char **argv, char **col_name)
{

    if (argc == 0)
        return 0;

    uint32_t *company_ids         = owned_company_ids->elements;
    uint32_t *player_ids          = owned_player_ids->elements;
    uint32_t *owned_stocks_amount = owned_stocks->elements;

    uint32_t stock_owned_amount = (uint32_t)atoi(argv[0]);
    uint32_t company_id         = (uint32_t)atoi(argv[1]);
    uint32_t player_id          = (uint32_t)atoi(argv[2]);

    for (size_t i = 0; i < owned_company_ids->num_elements;i++) {

        if (player_id == player_ids[i] && company_id == company_ids[i]) {

            owned_stocks_amount[i] = stock_owned_amount;
            break;

        }

    }

    return 0;

}

void InitOwnedStocks()
{

    Company *companies = GetAllCompanies();
    num_companies = GetNumCompanies();

    size_t num_player_ids = save_player_ids->num_elements;
    uint32_t *player_ids = save_player_ids->elements;

    uint32_t default_owned = 0;

    for (size_t i = 0; i < num_player_ids;i++) {

        for (size_t j = 0; j < num_companies;j++) {

            Vector_PushBack(owned_company_ids, &companies[j].company_id);
            Vector_PushBack(owned_stocks, &default_owned);
            Vector_PushBack(owned_player_ids, &player_ids[i]);

        }

    }

    char *query =   "SELECT SUM(PT.StocksExchanged), PT.CompanyId, PT.PlayerId FROM Player_Transactions PT "
                    "INNER JOIN Game_Players GP ON GP.PlayerId = PT.PlayerId "
                    "WHERE GP.SaveId = %d "
                    "GROUP BY PT.CompanyId, PT.PlayerId ";

    ExecuteQueryF(&GetOwnedStocks_CallBack, NULL, query, Game_GetSaveId());

}

int PlayerId_Callback(void *player, int argc, char **argv, char **col_name)
{

    if (argc == 0)
        return -1;

    int player_id = atoi(argv[0]);

    Vector_PushBack(save_player_ids, &player_id);

    return 0;

}

void InitPlayerIds()
{

    char *query = "SELECT PlayerId FROM Game_Players WHERE SaveId = %d";
    ExecuteQueryF(&PlayerId_Callback, NULL, query, Game_GetSaveId());

}

void dbaccount_save_transactions()
{

    Vector *vector = Queue_GetLockFreeVector(transaction_queue);

    sqlite3 *db;
    if (sqlite3_open_v2(DefaultConnection(), &db, SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK)
        return;

    ExecuteTransaction(db, vector);
    Vector_DeletePtrs(vector);

}

void InitVectors() 
{

    if (transactions == NULL) {

        transactions             = Vector_Create(sizeof(Transaction), 32);
        transaction_company_ids  = Vector_Create(sizeof(uint32_t), 32);
        transaction_player_ids   = Vector_Create(sizeof(uint32_t), 32); 

        owned_stocks      = Vector_Create(sizeof(uint32_t), 32);
        owned_company_ids = Vector_Create(sizeof(uint32_t), 32);
        owned_player_ids  = Vector_Create(sizeof(uint32_t), 32);

        save_player_ids   = Vector_Create(sizeof(uint32_t), 4);

    } else {

        Vector_Reset(transactions);
        Vector_Reset(transaction_company_ids);
        Vector_Reset(transaction_player_ids);

        Vector_Reset(owned_stocks);
        Vector_Reset(owned_company_ids);
        Vector_Reset(owned_player_ids);

        Vector_Reset(save_player_ids);

    }

}

void dbaccount_init()
{

    InitVectors();
    InitPlayerIds();
    InitOwnedStocks();
    InitSavedTransactions();
    transaction_queue = Queue_Create();

}