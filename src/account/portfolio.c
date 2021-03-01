
#include "portfolio.h"
#include "account.h"
#include "simulation.h"

#include "dbcompany.h"
#include "dbaccount.h"
#include "dbsave.h"

#include "vector.h"

float portfolio_get_asset_worth(uint32_t player_id)
{

	Vector *companies = GetAllCompaniesVector();
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

	return Account_GetMoney(player_id) + portfolio_get_asset_worth(player_id);

}