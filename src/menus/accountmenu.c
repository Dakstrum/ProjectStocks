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

int history_display_number = 0;

static MenuWithChilds *account_menu = NULL;

static DrawObject *company_name_textobject       = NULL;
static DrawObject *account_money_textobject      = NULL;
static DrawObject *account_date_textobject       = NULL; 
static DrawObject *stock_price_textobject        = NULL;
static DrawObject *owned_stock_amount_textobject = NULL;

static DrawObject *action_objects[DSP_NUM];
static DrawObject *date_objects[DSP_NUM];
static DrawObject *share_amount_objects[DSP_NUM];
static DrawObject *share_price_objects[DSP_NUM];
static DrawObject *transaction_objects[DSP_NUM];

char* GetTransactionAction(TransactionType type);
void InitalizeCompanyScrollbox();
void PopulateHistoryDisplay(char* company);
void InitializeHistoryDisplay();
void AccountMenuRenderLogic();
void InitalizeAccountMenuText();

void InitializeAccountMenu() 
{

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: account Menu could not create new draw layer");
        return;
    }

    account_menu = GetJSONMenuAndAddToDrawLayer("AccountMenu");

    InitalizeCompanyScrollbox();
    InitalizeAccountMenuText();
    InitializeSpeedSelectObject();

    InitializeHistoryDisplay();
    PopulateHistoryDisplay(GetCompanyNameViewing());

    AccountMenuRenderLogic(); 

}

void AccountMenuRenderLogic()
{

    if (account_money_textobject == NULL)
        return;
    
    SetTextContent(account_money_textobject,      "%.2f", GetAccountMoney());
    SetTextContent(account_date_textobject,       "%s",   GetDate());
    SetTextContent(stock_price_textobject,        "%.2f", CurrentStockPrice(GetCompanyNameViewing()));
    SetTextContent(owned_stock_amount_textobject, "%d",   GetOwnedStockAmount(GetCompanyNameViewing()));

}

void InitalizeAccountMenuText()
{

    company_name_textobject       = GetJSONObjectAndAddToDrawLayer("AccountMenuChangingCompanyNameText");
    account_money_textobject      = GetJSONObjectAndAddToDrawLayer("StocksMenuAccountMoneyText");
    account_date_textobject       = GetJSONObjectAndAddToDrawLayer("StocksMenuAccountDateText");
    stock_price_textobject        = GetJSONObjectAndAddToDrawLayer("AccountMenuCurrentStockPriceText");
    owned_stock_amount_textobject = GetJSONObjectAndAddToDrawLayer("AccountMenuOwnedStockAmountText");

    SetTextContent(company_name_textobject, "%s", GetCompanyNameViewing());

}

void InitializeHistoryDisplay()
{

    static char *action_json_objects[DSP_NUM]       = {"AccountMenuActionTextOne", "AccountMenuActionTextTwo", "AccountMenuActionTextThree", "AccountMenuActionTextFour", "AccountMenuActionTextFive"};
    static char *date_json_objects[DSP_NUM]         = {"AccountMenuDateTextOne", "AccountMenuDateTextTwo", "AccountMenuDateTextThree", "AccountMenuDateTextFour", "AccountMenuDateTextFive"};
    static char *share_amount_json_objects[DSP_NUM] = {"AccountMenuSharesTextOne", "AccountMenuSharesTextTwo", "AccountMenuSharesTextThree", "AccountMenuSharesTextFour", "AccountMenuSharesTextFive"};
    static char *share_price_json_objects[DSP_NUM]  = {"AccountMenuPerShareTextOne", "AccountMenuPerShareTextTwo", "AccountMenuPerShareTextThree", "AccountMenuPerShareTextFour", "AccountMenuPerShareTextFive"};
    static char *transaction_json_objects[DSP_NUM]  = {"AccountMenuTransactionTextOne", "AccountMenuTransactionTextTwo", "AccountMenuTransactionTextThree", "AccountMenuTransactionTextFour", "AccountMenuTransactionTextFive"};

    for (int i=0; i < DSP_NUM; i++) {

        action_objects[i]       = GetJSONObjectAndAddToDrawLayer(action_json_objects[i]);
        date_objects[i]         = GetJSONObjectAndAddToDrawLayer(date_json_objects[i]);
        share_amount_objects[i] = GetJSONObjectAndAddToDrawLayer(share_amount_json_objects[i]);
        share_price_objects[i]  = GetJSONObjectAndAddToDrawLayer(share_price_json_objects[i]);
        transaction_objects[i]  = GetJSONObjectAndAddToDrawLayer(transaction_json_objects[i]);

    }

}

void PopulateHistoryDisplay(char* company)
{
    
    struct Transactions *transaction = GetTransactions(company);

    char transaction_time[128];

    for (int i=0; i < DSP_NUM; i++) {

        if(transaction->shares[history_display_number + i]) {

            time_t time_buf = transaction->date[history_display_number + i];
            strftime(transaction_time, 128, "%x", localtime(&time_buf));

            SetTextContent(action_objects[i],       "%s",   GetTransactionAction(transaction->type[history_display_number + i]));
            SetTextContent(date_objects[i],         "%s",   transaction_time);
            SetTextContent(share_amount_objects[i], "%d",   transaction->shares[history_display_number + i]);
            SetTextContent(share_price_objects[i],  "%.2f", transaction->pershare[history_display_number + i]);
            SetTextContent(transaction_objects[i],  "%.2f", transaction->transaction[history_display_number + i]);

        }
        
    }

    free(transaction->pershare);
    free(transaction->shares);
    free(transaction->transaction);
    free(transaction->date);
    free(transaction->type);
    free(transaction);
    
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

        SetTextContent(action_objects[i],       "%s", "");
        SetTextContent(date_objects[i],         "%s", "");
        SetTextContent(share_amount_objects[i], "%s", "");
        SetTextContent(share_price_objects[i],  "%s", "");
        SetTextContent(transaction_objects[i],  "%s", "");

    }

}

void PopulateCompanyScrollBox(DrawObject *object)
{

    for(int i = 0; i < GetAmountOfCompanies(); i++)
        object->scrollbox.text_content[i]  = GetStockNameFromStockId(i+1);

}

void CompanyScrollBoxClick(char *scroll_box_content, unsigned short int index)
{

    SetCompanyIdViewing(GetCompanyId(scroll_box_content));
    ClearAccountHistoryDisplay();
    PopulateHistoryDisplay(scroll_box_content);

    SetTextContent(company_name_textobject, "%s", scroll_box_content);

}

void InitalizeCompanyScrollbox() 
{

    DrawObject *object = CreateScrollBoxObject();

    object->x          = 2;
    object->y          = 230;
    object->width      = 288;
    object->height     = 603;
    object->asset_path = "assets/images/companyicons/StocksBox.png";

    object->scrollbox.num_items    = GetAmountOfCompanies();
    object->scrollbox.box_click    = &CompanyScrollBoxClick;
    object->scrollbox.text_content = malloc(sizeof(char *) * 2);

    PopulateCompanyScrollBox(object);
    AddObjectToDrawLayer(object);

}


void AccountDown_BCB()
{

    history_display_number += DSP_NUM;
    ClearAccountHistoryDisplay();
    PopulateHistoryDisplay(GetCompanyNameViewing());

}

void AccountUp_BCB()
{
    if(history_display_number >= DSP_NUM) {

        history_display_number -= DSP_NUM;
        ClearAccountHistoryDisplay();
        PopulateHistoryDisplay(GetCompanyNameViewing());

    }
}

void CleanAccountMenu()
{

    history_display_number = 0;

    account_menu                  = NULL;
    company_name_textobject       = NULL;
    account_money_textobject      = NULL;
    account_date_textobject       = NULL; 
    stock_price_textobject        = NULL;
    owned_stock_amount_textobject = NULL;

}