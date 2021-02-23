
#include <stdint.h>

#include "account.h"
#include "dbaccount.h"
#include "transaction.h"

#include "game.h"
#include "simulation.h"
#include "simulation_modifier.h"

bool transaction_purchase_stocks(uint32_t player_id, uint32_t company_id, uint32_t stock_amount)
{

    float current_stock_price = Simulation_GetLastStockPriceByCompanyId(company_id);
    float total_price = stock_amount * current_stock_price;
    if (Account_CanMakeTransaction(player_id, total_price)) {

        dbaccount_buy_stocks(player_id, company_id, stock_amount, current_stock_price);
        Account_SubtractMoney(player_id, total_price);
        simulation_apply_transaction(stock_amount, company_id, Game_GetGameTime());

    } else {

        return false;

    }

    return true;

}

bool transaction_sell_stocks(uint32_t player_id, uint32_t company_id, uint32_t stock_amount)
{

    if (dbaccount_can_sell_stock(player_id, company_id, stock_amount)) {

        float current_stock_price = Simulation_GetLastStockPriceByCompanyId(company_id);
        dbaccount_sell_stocks(player_id, company_id, stock_amount, current_stock_price);
        Account_AddMoney(player_id, stock_amount * current_stock_price);
        simulation_apply_transaction(-((int)stock_amount), company_id, Game_GetGameTime());

    } else {

        return false;

    }

    return true;

}