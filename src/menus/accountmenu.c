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
#include "generalpurposemenus.h"
#include "drawlayerutils.h"

#define DSP_NUM 5

int HistoryDisplayNumber = 0;

static MenuWithChilds *account_menu           = NULL;

static DrawObject *CompanyNameTextObject      = NULL;
static DrawObject *AccountMoneyTextObject     = NULL;
static DrawObject *AccountDateTextObject      = NULL; 
static DrawObject *StockPriceTextObject       = NULL;
static DrawObject *OwnedStockAmountTextObject = NULL;

static DrawObject *ActionObjects[DSP_NUM];
static DrawObject *DateObjects[DSP_NUM];
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

    account_menu = GetJSONMenuAndAddToDrawLayer("AccountMenu");

    DisplayAccountCompanyScrollBox();
    InitializeAccountHistoryDisplay();
    PopulateAccountHistoryDisplay(GetCompanyNameViewing());

    InitalizeAccountMenuText();
    AccountMenuRenderLogic();
    InitializeSpeedSelectObject();

}

void AccountMenuRenderLogic()
{

    if (AccountMoneyTextObject == NULL)
        return;
    
    SetTextContent(AccountMoneyTextObject,     "%.2f", GetAccountMoney());
    SetTextContent(AccountDateTextObject,      "%s",   GetDate());
    SetTextContent(StockPriceTextObject,       "%.2f", CurrentStockPrice(GetCompanyNameViewing()));
    SetTextContent(OwnedStockAmountTextObject, "%d",   GetOwnedStockAmount(GetCompanyNameViewing()));

}

void InitalizeAccountMenuText()
{

    CompanyNameTextObject        = GetJSONObjectAndAddToDrawLayer("AccountMenuChangingCompanyNameText");
    AccountMoneyTextObject       = GetJSONObjectAndAddToDrawLayer("StocksMenuAccountMoneyText");
    AccountDateTextObject        = GetJSONObjectAndAddToDrawLayer("StocksMenuAccountDateText");
    StockPriceTextObject         = GetJSONObjectAndAddToDrawLayer("AccountMenuCurrentStockPriceText");
    OwnedStockAmountTextObject   = GetJSONObjectAndAddToDrawLayer("AccountMenuOwnedStockAmountText");

    SetTextContent(CompanyNameTextObject, "%s", GetCompanyNameViewing());

}

void InitializeAccountHistoryDisplay()
{

    static char *ActionJsonObjects[DSP_NUM]      = {"AccountMenuActionTextOne", "AccountMenuActionTextTwo", "AccountMenuActionTextThree", "AccountMenuActionTextFour", "AccountMenuActionTextFive"};
    static char *DateJsonObjects[DSP_NUM]        = {"AccountMenuDateTextOne", "AccountMenuDateTextTwo", "AccountMenuDateTextThree", "AccountMenuDateTextFour", "AccountMenuDateTextFive"};
    static char *SharesJsonObjects[DSP_NUM]      = {"AccountMenuSharesTextOne", "AccountMenuSharesTextTwo", "AccountMenuSharesTextThree", "AccountMenuSharesTextFour", "AccountMenuSharesTextFive"};
    static char *PerShareJsonObjects[DSP_NUM]    = {"AccountMenuPerShareTextOne", "AccountMenuPerShareTextTwo", "AccountMenuPerShareTextThree", "AccountMenuPerShareTextFour", "AccountMenuPerShareTextFive"};
    static char *TransactionJsonObjects[DSP_NUM] = {"AccountMenuTransactionTextOne", "AccountMenuTransactionTextTwo", "AccountMenuTransactionTextThree", "AccountMenuTransactionTextFour", "AccountMenuTransactionTextFive"};

    for (int i=0; i < DSP_NUM; i++) {

        ActionObjects[i]      = GetJSONObjectAndAddToDrawLayer(ActionJsonObjects[i]);
        DateObjects[i]        = GetJSONObjectAndAddToDrawLayer(DateJsonObjects[i]);
        SharesObjects[i]      = GetJSONObjectAndAddToDrawLayer(SharesJsonObjects[i]);
        PerShareObjects[i]    = GetJSONObjectAndAddToDrawLayer(PerShareJsonObjects[i]);
        TransactionObjects[i] = GetJSONObjectAndAddToDrawLayer(TransactionJsonObjects[i]);

    }

}

void PopulateAccountHistoryDisplay(char* company)
{
    
    struct Transactions *transaction = GetTransactions(company);

    char transaction_time[128];

    for (int i=0; i < DSP_NUM; i++) {

        if(transaction->shares[HistoryDisplayNumber + i]) {

            time_t time_buf = transaction->date[HistoryDisplayNumber + i];
            strftime(transaction_time, 128, "%x", localtime(&time_buf));

            SetTextContent(ActionObjects[i], "%s", GetTransactionAction(transaction->type[HistoryDisplayNumber + i]));
            SetTextContent(DateObjects[i], "%s", transaction_time);
            SetTextContent(SharesObjects[i], "%d", transaction->shares[HistoryDisplayNumber + i]);
            SetTextContent(PerShareObjects[i], "%.2f", transaction->pershare[HistoryDisplayNumber + i]);
            SetTextContent(TransactionObjects[i], "%.2f", transaction->transaction[HistoryDisplayNumber + i]);

        }
        
    }

    free(transaction->pershare);
    free(transaction->shares);
    free(transaction->transaction);
    free(transaction->date);
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
        SetTextContent(DateObjects[i], "%s", "");
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

void LoadAccountCompanyScrollBoxClick(char *scroll_box_content, unsigned short int index)
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
    PopulateAccountHistoryDisplay(GetCompanyNameViewing());

}

void AccountUp_BCB()
{
    if(HistoryDisplayNumber >= DSP_NUM) {

        HistoryDisplayNumber -= DSP_NUM;
        ClearAccountHistoryDisplay();
        PopulateAccountHistoryDisplay(GetCompanyNameViewing());

    }
}

void CleanAccountMenu()
{

    HistoryDisplayNumber = 0;

    account_menu               = NULL;
    CompanyNameTextObject      = NULL;
    AccountMoneyTextObject     = NULL;
    AccountDateTextObject      = NULL; 
    StockPriceTextObject       = NULL;
    OwnedStockAmountTextObject = NULL;

}