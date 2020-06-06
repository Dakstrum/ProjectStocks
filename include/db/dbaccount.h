#ifndef DBACCOUNT_H
#define DBACCOUNT_H

#include <sqlite3.h>

#include "graph.h"
#include "shared.h"

typedef enum TransactionType
{

	BUY,
	SELL

} TransactionType;

typedef struct Transactions
{

	int *id;
	TransactionType *type;
    time_t *date;
    int *shares;
    float *pershare;
    float *transaction;

    unsigned int num_transactions;
    unsigned int size;

} Transaction;


void InsertStockPrice(int save_id, int company_id, float stock_price, char *timestamp, sqlite3 *db);
StockPrices *GetStockPricesBetweenRange(char *company_name, char *start_time, char *end_time, TimeSpan timespan);

bool AttemptToSubtractFromCurrentStock(char *company_name, int amount_to_subtract, float price_per_stock);
void AttemptToAddFromCurrentStock(char *company_name, int amount_to_add, float price_per_stock);

char *GetSaveNameFromSaveId(int save_id);
int GetAmountOfSaves();

char *GetCompanyNameFromCompanyId(int stock_id);
int GetAmountOfCompanies();

char *GetPlayerNameFromSaveName(char *save_name);

struct Transactions *GetTransactions(char* company);
struct Transactions *GetAllTransactions();

int GetOwnedStockAmount(char *company_name);

void SetDBMoneyToLocalMoney(int player_id);

#endif