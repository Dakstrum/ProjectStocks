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

Vector *GetCompanyTransactions(uint32_t player_id, char *company);
Vector *dbaccount_get_player_transactions(uint32_t player_id);
Vector *dbaccount_get_all_transactions();
void dbaccount_save_transactions();

int dbaccount_get_owned_stock_amount(uint32_t player_id, uint32_t company_id);
void InitializeAccountInformation();

bool dbaccount_can_sell_stock(uint32_t player_id, uint32_t company_id, int amount_to_subtract);

#endif