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
    int *company_id;
	TransactionType *type;
    time_t *date;
    int *shares;
    float *pershare;
    float *transaction;

    unsigned int num_transactions;
    unsigned int size;

} Transactions;

bool AttemptToSubtractFromCurrentStock(char *company_name, int amount_to_subtract, float price_per_stock);
void AttemptToAddFromCurrentStock(char *company_name, int amount_to_add, float price_per_stock);

char *GetSaveNameFromSaveId(int save_id);
char *GetPlayerNameFromSaveName(char *save_name);

Transactions *GetCompanyTransactions(char *company);
Transactions *GetAllTransactions();
void FreeTransactions(Transactions *transactions);
void SaveTransactions();

int GetOwnedStockAmount(char *company_name);
void InitializeAccountInformation();

#endif