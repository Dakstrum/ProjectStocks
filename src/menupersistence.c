#include <stdio.h>
#include <stdlib.h>
#include "dbcompany.h"

static int companyid_viewing      = 1;
static int account_history_display_number = 0;

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