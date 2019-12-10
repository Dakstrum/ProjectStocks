#ifndef DBACCESS_H
#define DBACCESS_H

#include <sqlite3.h>

#include "graph.h"
#include "shared.h"

void InitializeDatabases();
void CopyPersistentToMemory();
int OpenConnection(sqlite3 **db, char *connection_string);
WindowSettings GetWindowSettingsFromDB();

char *MemoryConnection();
char *DefaultConnection();

int InsertAndOrSetCompanyToActive(char *company_name, float ipo);
unsigned int GetSaveSeedWithSaveId(int save_id);
int InsertSave(char *save_name, char *player_name, unsigned int game_seed);
void InsertStockPrice(int save_id, int company_id, float stock_price, char *timestamp, sqlite3 *db);
StockPrices *GetStockPricesBetweenRange(char *company_name, char *start_time, char *end_time, TimeSpan timespan);

void AttemptToSubtractFromCurrentStock(char *company_name, int amount_to_subtract, int price_per_stock);
void AttemptToAddFromCurrentStock(char *company_name, int amount_to_add, int price_per_stock);

#endif