#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"
#include "dbaccount.h"
#include "text.h"
void PopulateAccountHistoryDisplay();

static MenuWithChilds *account_menu = NULL;

static DrawObject *AccountMenuActionTextOne       = NULL;
static DrawObject *AccountMenuSharesTextOne       = NULL;
static DrawObject *AccountMenuPerShareTextOne     = NULL;
static DrawObject *AccountMenuTransactionTextOne  = NULL;

void InitializeAccountMenu() 
{

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: account Menu could not create new draw layer");
        return;
    }


    account_menu = GetMenuWithChildsFromJsonLayer("AccountMenu");
    AddMenuWithChildsToDrawLayer(account_menu);

    AccountMenuActionTextOne       = GetDrawObjectFromJsonLayer("AccountMenuActionTextOne");
    AccountMenuSharesTextOne       = GetDrawObjectFromJsonLayer("AccountMenuSharesTextOne");
    AccountMenuPerShareTextOne     = GetDrawObjectFromJsonLayer("AccountMenuPerShareTextOne");
    AccountMenuTransactionTextOne  = GetDrawObjectFromJsonLayer("AccountMenuTransactionTextOne");
    AddObjectToDrawLayer(AccountMenuActionTextOne);
    AddObjectToDrawLayer(AccountMenuSharesTextOne);
    AddObjectToDrawLayer(AccountMenuPerShareTextOne);
    AddObjectToDrawLayer(AccountMenuTransactionTextOne);


    PopulateAccountHistoryDisplay();

}

void CleanUpAccountMenu() 
{

    if (account_menu != NULL)
        free(account_menu);

    account_menu = NULL;
    
}

void PopulateAccountHistoryDisplay()
{

	struct Transactions *transaction = GetTransaction();
	SetTextContent(AccountMenuActionTextOne, "%d", transaction->type);
	SetTextContent(AccountMenuSharesTextOne, "%d", transaction->shares[0]);
	//SetTextContent(AccountMenuPerShareTextOne, "%d", transaction->pershare[0]);
	SetTextContent(AccountMenuTransactionTextOne, "%.2f", transaction->transaction[0]);


}

void AccountDownButtonCallBack()
{

	Log("Down");

}

void AccountUpButtonCallBack()
{

	Log("Up");

}