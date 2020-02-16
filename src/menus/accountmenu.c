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

void PopulateAccountHistoryDisplay()
{

	struct Transactions *transaction = GetTransaction();
    
    if(transaction->shares[0])
    {

        SetTextContent(AccountMenuActionTextOne, "%d", transaction->type[0]);
        SetTextContent(AccountMenuSharesTextOne, "%d", transaction->shares[0]);
        SetTextContent(AccountMenuTransactionTextOne, "%.2f", transaction->transaction[0]);

    }

    if(transaction->shares[1])
    {

        SetTextContent(AccountMenuActionTextTwo, "%d", transaction->type[1]);
        SetTextContent(AccountMenuSharesTextTwo, "%d", transaction->shares[1]);
        SetTextContent(AccountMenuTransactionTextTwo, "%.2f", transaction->transaction[1]);

    }

    if(transaction->shares[2])
    {

        SetTextContent(AccountMenuActionTextThree, "%d", transaction->type[2]);
        SetTextContent(AccountMenuSharesTextThree, "%d", transaction->shares[2]);
        SetTextContent(AccountMenuTransactionTextThree, "%.2f", transaction->transaction[2]);

    }

    if(transaction->shares[3])
    {

        SetTextContent(AccountMenuActionTextFour, "%d", transaction->type[3]);
        SetTextContent(AccountMenuSharesTextFour, "%d", transaction->shares[3]);
        SetTextContent(AccountMenuTransactionTextFour, "%.2f", transaction->transaction[3]);

    }

    if(transaction->shares[4])
    {

        SetTextContent(AccountMenuActionTextFive, "%d", transaction->type[4]);
        SetTextContent(AccountMenuSharesTextFive, "%d", transaction->shares[4]);
        SetTextContent(AccountMenuTransactionTextFive, "%.2f", transaction->transaction[4]);

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