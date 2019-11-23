#ifndef DBACCESS_H
#define DBACCESS_H

#include <sqlite3.h>

#include "graph.h"
#include "shared.h"

void InitializeDatabases();
void CopyPersistentToMemory();
int OpenConnection(sqlite3 **db, char *connection_string);
WindowSettings GetWindowSettings();

char *MemoryConnection();
char *DefaultConnection();

int InsertAndOrSetCompanyToActive(char *company_name, float ipo);
unsigned int GetSaveSeedWithSaveId(int save_id);
int InsertSave(char *save_name, char *player_name, unsigned int game_seed);
void InsertStockPrice(int save_id, int company_id, float stock_price, char *timestamp, sqlite3 *db);
StockPrices *GetStockPricesBetweenRange(char *company_name, char *start_time, char *end_time, TimeSpan timespan);

void InsertNewStockTransactionIntoOwnedStocks(int owned_stock_id, int save_id, int company_id, int *how_many_owned);
void InsertNewStockTransaction(int transation_id, int save_id, int player_name, int company_id, int *transaction_amount, int stocks_exchanged, time_t transaction_time);
void InsertSubtractedStockTransaction(int amount_to_subtract);
#endif