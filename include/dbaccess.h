#ifndef DBACCESS_H
#define DBACCESS_H

#include <sqlite3.h>

#include "graph.h"

typedef struct StockPrices {

    float *prices;
    unsigned int num_prices;
    unsigned int size;

} StockPrices;

void InitializeDatabases();
int OpenConnection(sqlite3 **db);

int InsertAndOrSetCompanyToActive(char *company_name, float ipo);
void InsertStockPrice(int save_id, int company_id, float stock_price, char *timestamp, sqlite3 *db);
StockPrices *GetStockPricesBetweenRange(char *company_name, char *start_time, char *end_time, TimeSpan timespan);

#endif