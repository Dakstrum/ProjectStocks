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

#define DSP_NUM 5

char* GetTransactionAction(TransactionType type);
void DisplayAccountCompanyScrollBox();
void PopulateAccountHistoryDisplay();
void InitializeAccountHistoryDisplay();

int HistoryDisplayNumber = 0;

static MenuWithChilds *account_menu = NULL;

static char *ActionJsonObjects[DSP_NUM]      = {"AccountMenuActionTextOne", "AccountMenuActionTextTwo", "AccountMenuActionTextThree", "AccountMenuActionTextFour", "AccountMenuActionTextFive"};
static char *SharesJsonObjects[DSP_NUM]      = {"AccountMenuSharesTextOne", "AccountMenuSharesTextTwo", "AccountMenuSharesTextThree", "AccountMenuSharesTextFour", "AccountMenuSharesTextFive"};
static char *PerShareJsonObjects[DSP_NUM]    = {"AccountMenuPerShareTextOne", "AccountMenuPerShareTextTwo", "AccountMenuPerShareTextThree", "AccountMenuPerShareTextFour", "AccountMenuPerShareTextFive"};
static char *TransactionJsonObjects[DSP_NUM] = {"AccountMenuTransactionTextOne", "AccountMenuTransactionTextTwo", "AccountMenuTransactionTextThree", "AccountMenuTransactionTextFour", "AccountMenuTransactionTextFive"};

DrawObject *ActionObjects[DSP_NUM];
DrawObject *SharesObjects[DSP_NUM];
DrawObject *PerShareObjects[DSP_NUM];
DrawObject *TransactionObjects[DSP_NUM];

void InitializeAccountMenu() 
{

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: account Menu could not create new draw layer");
        return;
    }

    account_menu = GetMenuWithChildsFromJsonLayer("AccountMenu");
    AddMenuWithChildsToDrawLayer(account_menu);

    DisplayAccountCompanyScrollBox();
    InitializeAccountHistoryDisplay();
    PopulateAccountHistoryDisplay();

}

void InitializeAccountHistoryDisplay()
{

    for (int i=0; i < DSP_NUM; i++) {

        ActionObjects[i]      = GetDrawObjectFromJsonLayer(ActionJsonObjects[i]);
        SharesObjects[i]      = GetDrawObjectFromJsonLayer(SharesJsonObjects[i]);
        PerShareObjects[i]    = GetDrawObjectFromJsonLayer(PerShareJsonObjects[i]);
        TransactionObjects[i] = GetDrawObjectFromJsonLayer(TransactionJsonObjects[i]);

        AddObjectToDrawLayer(ActionObjects[i]);
        AddObjectToDrawLayer(SharesObjects[i]);
        AddObjectToDrawLayer(PerShareObjects[i]);
        AddObjectToDrawLayer(TransactionObjects[i]);

    }

}

void PopulateAccountHistoryDisplay()
{

    struct Transactions *transaction[DSP_NUM];

    for (int i=0; i < DSP_NUM; i++) {

        transaction[i] = GetTransaction();
        if(transaction[i]->shares[HistoryDisplayNumber + i]) {

            SetTextContent(ActionObjects[i], "%s", GetTransactionAction(transaction[i]->type[HistoryDisplayNumber + i]));
            SetTextContent(SharesObjects[i], "%d", transaction[i]->shares[HistoryDisplayNumber + i]);
            SetTextContent(PerShareObjects[i], "%d", transaction[i]->pershare[HistoryDisplayNumber + i]);
            SetTextContent(TransactionObjects[i], "%.2f", transaction[i]->transaction[HistoryDisplayNumber + i]);

        }
        
    }

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

void ClearAccountHistoryDisplay()
{

    for (int i=0; i < DSP_NUM; i++) {

        SetTextContent(ActionObjects[i], "%s", "");
        SetTextContent(SharesObjects[i], "%s", "");
        SetTextContent(PerShareObjects[i], "%s", "");
        SetTextContent(TransactionObjects[i], "%s", "");

    }

}

void AddAccountCompanyContentToStocksScrollBox(DrawObject *object)
{

    for(int i = 0; i < GetAmountOfCompanies(); i++)
        object->scrollbox.text_content[i]  = GetStockNameFromStockId(i+1);

}

void LoadAccountCompanyScrollBoxClick(char *scroll_box_content)
{

    LogF("COMPANY SELECTED: %s", scroll_box_content);

}

void DisplayAccountCompanyScrollBox() 
{

    DrawObject *object = CreateScrollBoxObject();

    object->x          = 2;
    object->y          = 230;
    object->width      = 288;
    object->height     = 603;
    object->asset_path = "assets/images/companyicons/StocksBox.png";

    object->scrollbox.num_items    = GetAmountOfCompanies();
    object->scrollbox.box_click    = &LoadAccountCompanyScrollBoxClick;
    object->scrollbox.text_content = malloc(sizeof(char *) * 2);

    AddAccountCompanyContentToStocksScrollBox(object);
    AddObjectToDrawLayer(object);

}


void AccountDown_BCB()
{

    HistoryDisplayNumber += DSP_NUM;
    ClearAccountHistoryDisplay();
    PopulateAccountHistoryDisplay();

}

void AccountUp_BCB()
{
    if(HistoryDisplayNumber >= DSP_NUM) {

        HistoryDisplayNumber -= DSP_NUM;
        ClearAccountHistoryDisplay();
        PopulateAccountHistoryDisplay();

    }
}