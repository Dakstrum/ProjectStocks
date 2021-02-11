#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>

#include "dbcompany.h"
#include "shared.h"
#include "menupersistence.h"

static int companyid_viewing      = 1;
static int account_history_display_number = 0;
static TimeSpan stock_timespan_number = ONE_DAY;

static atomic_int current_menu = MAIN;

//AllMenus

void SetCompanyIdViewing(int id)
{

    companyid_viewing = id;

}

int GetCompanyIdViewing()
{

    return companyid_viewing;

}

char *GetCompanyNameViewing()
{

    return GetCompanyNameRef(GetCompanyIdViewing());

}

//AccountMenu

void SetAccountHistoryDisplayNum(int num)
{

	account_history_display_number = num;

}

int GetAccountHistoryDisplayNum()
{

	return account_history_display_number;

}

//StocksMenu

void SetStockMenuTimeSpanNum(TimeSpan time_span)
{

	stock_timespan_number = time_span;

}

TimeSpan GetStockMenuTimeSpanNum()
{

	return stock_timespan_number;

}

void persistence_set_current_menu(MENUS menu)
{

	atomic_store(&current_menu, menu);

}

MENUS persistence_get_current_menu()
{

	return atomic_load(&current_menu);

}