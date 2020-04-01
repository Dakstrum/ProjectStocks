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
#include "scrollbox.h"
#include "account.h"
#include "simulation.h"

#define DSP_NUM 5

int HistoryDisplayNumber = 0;

static MenuWithChilds *account_menu           = NULL;

static DrawObject *CompanyNameTextObject      = NULL;
static DrawObject *AccountMoneyTextObject     = NULL;
static DrawObject *StockPriceTextObject       = NULL;
static DrawObject *OwnedStockAmountTextObject = NULL;

static DrawObject *ActionObjects[DSP_NUM];
static DrawObject *SharesObjects[DSP_NUM];
static DrawObject *PerShareObjects[DSP_NUM];
static DrawObject *TransactionObjects[DSP_NUM];

char* GetTransactionAction(TransactionType type);
void DisplayAccountCompanyScrollBox();
void PopulateAccountHistoryDisplay(char* company);
void InitializeAccountHistoryDisplay();
void AccountMenuRenderLogic();
void InitalizeAccountMenuText();

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
    PopulateAccountHistoryDisplay(GetStockNameFromStockId(GetCompanyIdViewing()));

    InitalizeAccountMenuText();
    AccountMenuRenderLogic();

}

void AccountMenuRenderLogic()
{

    if (AccountMoneyTextObject == NULL)
        return;
    
    SetTextContent(AccountMoneyTextObject,     "%.2f", GetAccountMoney());
    SetTextContent(StockPriceTextObject,       "%.2f", CurrentStockPrice(GetStockNameFromStockId(GetCompanyIdViewing())));
    SetTextContent(OwnedStockAmountTextObject, "%d",   GetOwnedStockAmount(GetStockNameFromStockId(GetCompanyIdViewing())));

}

void InitalizeAccountMenuText()
{

    CompanyNameTextObject        = GetObjectAndDraw("AccountMenuChangingCompanyNameText");
    AccountMoneyTextObject       = GetObjectAndDraw("StocksMenuAccountMoneyText");
    StockPriceTextObject         = GetObjectAndDraw("AccountMenuCurrentStockPriceText");
    OwnedStockAmountTextObject   = GetObjectAndDraw("AccountMenuOwnedStockAmountText");

    SetTextContent(CompanyNameTextObject, "%s", GetStockNameFromStockId(GetCompanyIdViewing()));

}

void InitializeAccountHistoryDisplay()
{

    static char *ActionJsonObjects[DSP_NUM]      = {"AccountMenuActionTextOne", "AccountMenuActionTextTwo", "AccountMenuActionTextThree", "AccountMenuActionTextFour", "AccountMenuActionTextFive"};
    static char *SharesJsonObjects[DSP_NUM]      = {"AccountMenuSharesTextOne", "AccountMenuSharesTextTwo", "AccountMenuSharesTextThree", "AccountMenuSharesTextFour", "AccountMenuSharesTextFive"};
    static char *PerShareJsonObjects[DSP_NUM]    = {"AccountMenuPerShareTextOne", "AccountMenuPerShareTextTwo", "AccountMenuPerShareTextThree", "AccountMenuPerShareTextFour", "AccountMenuPerShareTextFive"};
    static char *TransactionJsonObjects[DSP_NUM] = {"AccountMenuTransactionTextOne", "AccountMenuTransactionTextTwo", "AccountMenuTransactionTextThree", "AccountMenuTransactionTextFour", "AccountMenuTransactionTextFive"};

    for (int i=0; i < DSP_NUM; i++) {

        ActionObjects[i]      = GetObjectAndDraw(ActionJsonObjects[i]);
        SharesObjects[i]      = GetObjectAndDraw(SharesJsonObjects[i]);
        PerShareObjects[i]    = GetObjectAndDraw(PerShareJsonObjects[i]);
        TransactionObjects[i] = GetObjectAndDraw(TransactionJsonObjects[i]);

    }

}

void PopulateAccountHistoryDisplay(char* company)
{
    
    struct Transactions *transaction = GetTransactions(company);

    for (int i=0; i < DSP_NUM; i++) {

        if(transaction->shares[HistoryDisplayNumber + i]) {

            SetTextContent(ActionObjects[i], "%s", GetTransactionAction(transaction->type[HistoryDisplayNumber + i]));
            SetTextContent(SharesObjects[i], "%d", transaction->shares[HistoryDisplayNumber + i]);
            SetTextContent(PerShareObjects[i], "%.2f", transaction->pershare[HistoryDisplayNumber + i]);
            SetTextContent(TransactionObjects[i], "%.2f", transaction->transaction[HistoryDisplayNumber + i]);

        }
        
    }

    free(transaction->pershare);
    free(transaction->shares);
    free(transaction->transaction);
    free(transaction->type);
    free(transaction);
    
}

void CleanUpAccountMenu() 
{

    if (account_menu != NULL)
        free(account_menu);

    account_menu = NULL;
    
}

char* GetTransactionAction(TransactionType type)
{

    if(type == 0)
        return "Buy";

    if(type == 1)
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

    SetCompanyIdViewing(GetCompanyId(scroll_box_content));
    ClearAccountHistoryDisplay();
    PopulateAccountHistoryDisplay(scroll_box_content);

    SetTextContent(CompanyNameTextObject, "%s", scroll_box_content);

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
    PopulateAccountHistoryDisplay(GetStockNameFromStockId(GetCompanyIdViewing()));

}

void AccountUp_BCB()
{
    if(HistoryDisplayNumber >= DSP_NUM) {

        HistoryDisplayNumber -= DSP_NUM;
        ClearAccountHistoryDisplay();
        PopulateAccountHistoryDisplay(GetStockNameFromStockId(GetCompanyIdViewing()));

    }
}