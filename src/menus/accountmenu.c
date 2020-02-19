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

int HistoryDisplayNumber = 0;

void PopulateAccountHistoryDisplay();

static MenuWithChilds *account_menu               = NULL;

static DrawObject *AccountMenuActionTextOne       = NULL;
static DrawObject *AccountMenuSharesTextOne       = NULL;
static DrawObject *AccountMenuPerShareTextOne     = NULL;
static DrawObject *AccountMenuTransactionTextOne  = NULL;

static DrawObject *AccountMenuActionTextTwo       = NULL;
static DrawObject *AccountMenuSharesTextTwo       = NULL;
static DrawObject *AccountMenuPerShareTextTwo     = NULL;
static DrawObject *AccountMenuTransactionTextTwo  = NULL;

static DrawObject *AccountMenuActionTextThree      = NULL;
static DrawObject *AccountMenuSharesTextThree      = NULL;
static DrawObject *AccountMenuPerShareTextThree    = NULL;
static DrawObject *AccountMenuTransactionTextThree = NULL;

static DrawObject *AccountMenuActionTextFour       = NULL;
static DrawObject *AccountMenuSharesTextFour       = NULL;
static DrawObject *AccountMenuPerShareTextFour     = NULL;
static DrawObject *AccountMenuTransactionTextFour  = NULL;

static DrawObject *AccountMenuActionTextFive       = NULL;
static DrawObject *AccountMenuSharesTextFive       = NULL;
static DrawObject *AccountMenuPerShareTextFive     = NULL;
static DrawObject *AccountMenuTransactionTextFive  = NULL;

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

    AccountMenuActionTextTwo       = GetDrawObjectFromJsonLayer("AccountMenuActionTextTwo");
    AccountMenuSharesTextTwo       = GetDrawObjectFromJsonLayer("AccountMenuSharesTextTwo");
    AccountMenuPerShareTextTwo     = GetDrawObjectFromJsonLayer("AccountMenuPerShareTextTwo");
    AccountMenuTransactionTextTwo  = GetDrawObjectFromJsonLayer("AccountMenuTransactionTextTwo");

    AccountMenuActionTextThree       = GetDrawObjectFromJsonLayer("AccountMenuActionTextThree");
    AccountMenuSharesTextThree       = GetDrawObjectFromJsonLayer("AccountMenuSharesTextThree");
    AccountMenuPerShareTextThree     = GetDrawObjectFromJsonLayer("AccountMenuPerShareTextThree");
    AccountMenuTransactionTextThree  = GetDrawObjectFromJsonLayer("AccountMenuTransactionTextThree");

    AccountMenuActionTextFour       = GetDrawObjectFromJsonLayer("AccountMenuActionTextFour");
    AccountMenuSharesTextFour       = GetDrawObjectFromJsonLayer("AccountMenuSharesTextFour");
    AccountMenuPerShareTextFour     = GetDrawObjectFromJsonLayer("AccountMenuPerShareTextFour");
    AccountMenuTransactionTextFour  = GetDrawObjectFromJsonLayer("AccountMenuTransactionTextFour");

    AccountMenuActionTextFive       = GetDrawObjectFromJsonLayer("AccountMenuActionTextFive");
    AccountMenuSharesTextFive       = GetDrawObjectFromJsonLayer("AccountMenuSharesTextFive");
    AccountMenuPerShareTextFive     = GetDrawObjectFromJsonLayer("AccountMenuPerShareTextFive");
    AccountMenuTransactionTextFive  = GetDrawObjectFromJsonLayer("AccountMenuTransactionTextFive");

    AddObjectToDrawLayer(AccountMenuActionTextOne);
    AddObjectToDrawLayer(AccountMenuSharesTextOne);
    AddObjectToDrawLayer(AccountMenuPerShareTextOne);
    AddObjectToDrawLayer(AccountMenuTransactionTextOne);

    AddObjectToDrawLayer(AccountMenuActionTextTwo);
    AddObjectToDrawLayer(AccountMenuSharesTextTwo);
    AddObjectToDrawLayer(AccountMenuPerShareTextTwo);
    AddObjectToDrawLayer(AccountMenuTransactionTextTwo);

    AddObjectToDrawLayer(AccountMenuActionTextThree);
    AddObjectToDrawLayer(AccountMenuSharesTextThree);
    AddObjectToDrawLayer(AccountMenuPerShareTextThree);
    AddObjectToDrawLayer(AccountMenuTransactionTextThree);

    AddObjectToDrawLayer(AccountMenuActionTextFour);
    AddObjectToDrawLayer(AccountMenuSharesTextFour);
    AddObjectToDrawLayer(AccountMenuPerShareTextFour);
    AddObjectToDrawLayer(AccountMenuTransactionTextFour);

    AddObjectToDrawLayer(AccountMenuActionTextFive);
    AddObjectToDrawLayer(AccountMenuSharesTextFive);
    AddObjectToDrawLayer(AccountMenuPerShareTextFive);
    AddObjectToDrawLayer(AccountMenuTransactionTextFive);

    PopulateAccountHistoryDisplay();

}

void CleanUpAccountMenu() 
{

    if (account_menu != NULL)
        free(account_menu);

    account_menu = NULL;
    
}

char* GetTransactionAction(TransactionType type)
{

    if(type == 1)
        return "Buy";

    if(type == 0)
        return "Sell";
    return "error";
}

void PopulateAccountHistoryDisplay()
{

	struct Transactions *transaction = GetTransaction();

    if(transaction->shares[0])
    {
        Log("0");
        SetTextContent(AccountMenuActionTextOne, "%s", GetTransactionAction(transaction->type[HistoryDisplayNumber]));
        SetTextContent(AccountMenuSharesTextOne, "%d", transaction->shares[HistoryDisplayNumber]);
        SetTextContent(AccountMenuTransactionTextOne, "%.2f", transaction->transaction[HistoryDisplayNumber]);

    }

    if(transaction->shares[1])
    {
        Log("1");
        SetTextContent(AccountMenuActionTextTwo, "%s", GetTransactionAction(transaction->type[HistoryDisplayNumber+1]));
        SetTextContent(AccountMenuSharesTextTwo, "%d", transaction->shares[HistoryDisplayNumber+1]);
        SetTextContent(AccountMenuTransactionTextTwo, "%.2f", transaction->transaction[HistoryDisplayNumber+1]);

    }

    if(transaction->shares[2])
    {
        Log("2");
        SetTextContent(AccountMenuActionTextThree, "%s", GetTransactionAction(transaction->type[HistoryDisplayNumber+2]));
        SetTextContent(AccountMenuSharesTextThree, "%d", transaction->shares[HistoryDisplayNumber+2]);
        SetTextContent(AccountMenuTransactionTextThree, "%.2f", transaction->transaction[HistoryDisplayNumber+2]);

    }

    if(transaction->shares[3])
    {
        Log("3");
        SetTextContent(AccountMenuActionTextFour, "%s", GetTransactionAction(transaction->type[HistoryDisplayNumber+3]));
        SetTextContent(AccountMenuSharesTextFour, "%d", transaction->shares[HistoryDisplayNumber+3]);
        SetTextContent(AccountMenuTransactionTextFour, "%.2f", transaction->transaction[HistoryDisplayNumber+3]);

    }

    if(transaction->shares[4])
    {
        Log("4");
        SetTextContent(AccountMenuActionTextFive, "%s", GetTransactionAction(transaction->type[HistoryDisplayNumber+4]));
        SetTextContent(AccountMenuSharesTextFive, "%d", transaction->shares[HistoryDisplayNumber+4]);
        SetTextContent(AccountMenuTransactionTextFive, "%.2f", transaction->transaction[HistoryDisplayNumber+4]);

    }

    //This is nasty. all of it :/ 
}

void AccountDownButtonCallBack()
{

	Log("Down");

}

void AccountUpButtonCallBack()
{

	Log("Up");

}