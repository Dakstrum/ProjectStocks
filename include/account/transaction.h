#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdint.h>

bool transaction_purchase_stocks(uint32_t player_id, uint32_t company_id, uint32_t stock_amount);
bool transaction_sell_stocks(uint32_t player_id, uint32_t company_id, uint32_t stock_amount);

#endif