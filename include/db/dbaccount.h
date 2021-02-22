#ifndef DBACCOUNT_H
#define DBACCOUNT_H

#include <stdint.h>

#include <sqlite3.h>

#include "graph.h"
#include "shared.h"
#include "vector.h"

typedef enum TransactionType
{

	BUY,
	SELL

} TransactionType;

typedef struct Transaction
{

    uint32_t transaction_id;
    uint32_t company_id;
    time_t transaction_date;
    int shares_exchanged;
    float price_per_share;
    float transaction_amount;
    TransactionType type;

} Transaction;

bool dbaccount_sell_stocks(uint32_t player_id, uint32_t company_id, int amount_to_subtract, float price_per_stock);
void dbaccount_buy_stocks(uint32_t player_id, uint32_t company_id, int amount_to_add, float price_per_stock);

char *GetSaveNameFromSaveId(int save_id);
char *GetPlayerNameFromSaveName(char *save_name);

Vector *GetCompanyTransactions(uint32_t player_id, char *company);
Vector *GetAllTransactions(uint32_t player_id);
void SaveTransactions();

int GetOwnedStockAmount(uint32_t player_id, uint32_t company_id);
void InitializeAccountInformation();
float GetAccountNetWorth(uint32_t player_id);

bool dbaccount_can_sell_stock(uint32_t player_id, uint32_t company_id, int amount_to_subtract);

#endif