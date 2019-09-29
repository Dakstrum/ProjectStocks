#ifndef DBACCESS_H
#define DBACCESS_H

#include <sqlite3.h>

void InitializeDatabases();
int InsertAndOrSetCompanyToActive(char *company_name, float ipo);
void InsertStockPrice(int save_id, int company_id, float stock_price, char *timestamp, sqlite3 *db);

#endif