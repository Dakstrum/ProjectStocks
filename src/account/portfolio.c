
#include "portfolio.h"
#include "account.h"
#include "simulation.h"

#include "dbcompany.h"
#include "dbaccount.h"
#include "dbsave.h"

#include "log.h"
#include "vector.h"

float portfolio_get_asset_worth(uint32_t player_id)
{

	Vector *companies = dbcompany_get_companies_vector();
	Company *companies_temp = companies->elements;

	float worth = 0.0f;
	for (size_t i = 0;i < companies->num_elements;i++) {

		int amount = dbaccount_get_owned_stock_amount(player_id, companies_temp[i].company_id);
		if (amount == 0)
			continue;

		worth += (float)amount * Simulation_GetLastStockPrice(companies_temp[i].company_name);

	}

	return worth;
}

float portfolio_get_networth(uint32_t player_id) 
{

	LogF("portfolio worth %u, %f", player_id, portfolio_get_asset_worth(player_id));
	return Account_GetMoney(player_id) + portfolio_get_asset_worth(player_id);

}

float portfolio_get_percentage(uint32_t player_id, uint32_t company_id)
{

	Vector *companies = dbcompany_get_companies_vector();
	Company *companies_temp = companies->elements;

	float specific_amount_owned  = 0.0f;
	float total_stocks_owned     = 0.0f;
	for (size_t i = 0;i < companies->num_elements;i++) {

		int amount = dbaccount_get_owned_stock_amount(player_id, companies_temp[i].company_id);
		if (company_id == companies_temp[i].company_id)
			specific_amount_owned = amount;

		total_stocks_owned += amount;

	}
	
	if (total_stocks_owned == 0.0f)
		return 0.0f;

	return specific_amount_owned/total_stocks_owned;

}

float portfolio_get_mean_purchase_price(uint32_t player_id, uint32_t company_id)
{

	Vector *transactions = dbaccount_get_company_transactions(player_id, company_id);

	uint32_t stocks_owned = 0;
	float total_transaction_amount = 0.0f;
	Vector_ForEach(i, transaction, transactions, Transaction *) {

		stocks_owned += transaction->shares_exchanged;
		total_transaction_amount += transaction->transaction_amount;

	}

	Vector_Delete(transactions);

	if (total_transaction_amount == 0.0f)
		return 0.0f;

	return stocks_owned/total_transaction_amount;

}