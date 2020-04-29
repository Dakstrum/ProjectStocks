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

	TransactionType *type;
    long int *date;
    int *shares;
    float *pershare;
    float *transaction;

    unsigned int num_transactions;
    unsigned int size;

} Transaction;



int InsertAndOrSetCompanyToActive(char *company_name, float ipo);
void InsertStockPrice(int save_id, int company_id, float stock_price, char *timestamp, sqlite3 *db);
StockPrices *GetStockPricesBetweenRange(char *company_name, char *start_time, char *end_time, TimeSpan timespan);

void AttemptToSubtractFromCurrentStock(char *company_name, int amount_to_subtract, float price_per_stock);
void AttemptToAddFromCurrentStock(char *company_name, int amount_to_add, float price_per_stock);

char *GetStockNameFromStockId(int stock_id);
int GetAmountOfCompanies();

char *GetSaveNameFromSaveId(int save_id);
int GetAmountOfSaves();

int GetAmountOfCompanies();
char *GetStockNameFromStockId(int stock_id);

char *GetPlayerNameFromSaveName(char *save_name);
int *GetSaveIdFromSaveName(char *save_name);

struct Transactions *GetTransactions(char* company);

int GetOwnedStockAmount(char *company_name);

int GetCompanyId(char *company_name);

void SetAccountMoneyToDBAmount(int player_id);
#endif