#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdint.h>

bool transaction_purchase_stock_amount(uint32_t player_id, uint32_t company_id, uint32_t stock_amount);
bool transaction_purchase_stock(uint32_t player_id, uint32_t company_id, float price);
bool transaction_sell_stock_amount(uint32_t player_id, uint32_t company_id, uint32_t stock_amount);

#endif