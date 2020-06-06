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

static DrawObject *player_money_textobject       = NULL;
static DrawObject *player_date_textobject        = NULL; 

static DrawObject *company_name_textobject       = NULL;
static DrawObject *stock_price_textobject        = NULL;
static DrawObject *owned_stock_amount_textobject = NULL;

//Selected Stock History Text Objects
static DrawObject *selected_action_objects[DSP_NUM];
static DrawObject *selected_date_objects[DSP_NUM];
static DrawObject *selected_share_amount_objects[DSP_NUM];
static DrawObject *selected_share_price_objects[DSP_NUM];
static DrawObject *selected_transaction_objects[DSP_NUM];

//All Stocks History Text Objects
static DrawObject *all_name_objects[DSP_NUM];
static DrawObject *all_action_objects[DSP_NUM];
static DrawObject *all_date_objects[DSP_NUM];
static DrawObject *all_share_amount_objects[DSP_NUM];


char* GetTransactionAction(TransactionType type);

void InitalizeAccountMenuCompanyScrollbox();
void PopulateSelectedStockHistoryDisplay(char* company);

void InitializeAllStocksHistoryDisplay();
void PopulateAllStocksHistoryDisplay();

void InitializeSelectedStockHistoryDisplay();
void AccountMenuRenderLogic();
void InitalizeAccountMenuText();

void InitializeAccountMenu() 
{

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: account Menu could not create new draw layer");
        return;
    }

    account_menu = GetJSONMenuAndAddToDrawLayer("AccountMenu");

    InitalizeAccountMenuCompanyScrollbox();
    InitalizeAccountMenuText();
    InitializeSpeedSelectObject();

    InitializeSelectedStockHistoryDisplay();
    PopulateSelectedStockHistoryDisplay(GetCompanyNameViewing());

    InitializeAllStocksHistoryDisplay();
    PopulateAllStocksHistoryDisplay();


    AccountMenuRenderLogic(); 

}

void AccountMenuRenderLogic()
{

    if (player_money_textobject == NULL)
        return;
    
    SetTextContent(player_money_textobject,       "%.2f", GetAccountMoney());
    SetTextContent(player_date_textobject,        "%s",   GetDate());
    SetTextContent(stock_price_textobject,        "%.2f", CurrentStockPrice(GetCompanyNameViewing())); //Mem leak
    SetTextContent(owned_stock_amount_textobject, "%d",   GetOwnedStockAmount(GetCompanyNameViewing()));

}

void InitalizeAccountMenuText()
{

    company_name_textobject       = GetJSONObjectAndAddToDrawLayer("AccountMenuChangingCompanyNameText");
    player_money_textobject       = GetJSONObjectAndAddToDrawLayer("StocksMenuAccountMoneyText");
    player_date_textobject        = GetJSONObjectAndAddToDrawLayer("StocksMenuAccountDateText");
    stock_price_textobject        = GetJSONObjectAndAddToDrawLayer("AccountMenuCurrentStockPriceText");
    owned_stock_amount_textobject = GetJSONObjectAndAddToDrawLayer("AccountMenuOwnedStockAmountText");

    SetTextContent(company_name_textobject, "%s", GetCompanyNameViewing());

}

void InitializeSelectedStockHistoryDisplay()
{

    static char *action_json_objects[DSP_NUM]       = {"AccountMenuActionTextOne", "AccountMenuActionTextTwo", "AccountMenuActionTextThree", "AccountMenuActionTextFour", "AccountMenuActionTextFive"};
    static char *date_json_objects[DSP_NUM]         = {"AccountMenuDateTextOne", "AccountMenuDateTextTwo", "AccountMenuDateTextThree", "AccountMenuDateTextFour", "AccountMenuDateTextFive"};
    static char *share_amount_json_objects[DSP_NUM] = {"AccountMenuSharesTextOne", "AccountMenuSharesTextTwo", "AccountMenuSharesTextThree", "AccountMenuSharesTextFour", "AccountMenuSharesTextFive"};
    static char *share_price_json_objects[DSP_NUM]  = {"AccountMenuPerShareTextOne", "AccountMenuPerShareTextTwo", "AccountMenuPerShareTextThree", "AccountMenuPerShareTextFour", "AccountMenuPerShareTextFive"};
    static char *transaction_json_objects[DSP_NUM]  = {"AccountMenuTransactionTextOne", "AccountMenuTransactionTextTwo", "AccountMenuTransactionTextThree", "AccountMenuTransactionTextFour", "AccountMenuTransactionTextFive"};

    for (int i=0; i < DSP_NUM; i++) {

        selected_action_objects[i]       = GetJSONObjectAndAddToDrawLayer(action_json_objects[i]);
        selected_date_objects[i]         = GetJSONObjectAndAddToDrawLayer(date_json_objects[i]);
        selected_share_amount_objects[i] = GetJSONObjectAndAddToDrawLayer(share_amount_json_objects[i]);
        selected_share_price_objects[i]  = GetJSONObjectAndAddToDrawLayer(share_price_json_objects[i]);
        selected_transaction_objects[i]  = GetJSONObjectAndAddToDrawLayer(transaction_json_objects[i]);

    }

}

void PopulateSelectedStockHistoryDisplay(char* company)
{
    
    struct Transactions *transaction = GetTransactions(company);

    char transaction_time[128];

    for (int i=0; i < DSP_NUM; i++) {

        if(transaction->shares[history_display_number + i]) {

            time_t time_buf = transaction->date[history_display_number + i];
            strftime(transaction_time, 128, "%x", localtime(&time_buf));

            SetTextContent(selected_action_objects[i],       "%s",   GetTransactionAction(transaction->type[history_display_number + i]));
            SetTextContent(selected_date_objects[i],         "%s",   transaction_time);
            SetTextContent(selected_share_amount_objects[i], "%d",   transaction->shares[history_display_number + i]);
            SetTextContent(selected_share_price_objects[i],  "%.2f", transaction->pershare[history_display_number + i]);
            SetTextContent(selected_transaction_objects[i],  "%.2f", transaction->transaction[history_display_number + i]);

        }
        
    }

    free(transaction->id);
    free(transaction->pershare);
    free(transaction->shares);
    free(transaction->transaction);
    free(transaction->date);
    free(transaction->type);
    free(transaction);
    
}

void InitializeAllStocksHistoryDisplay()
{

    static char *name_json_objects[DSP_NUM]         = {"AccountMenuNameOne",   "AccountMenuNameTwo",   "AccountMenuNameThree",   "AccountMenuNameFour",   "AccountMenuNameFive"};
    static char *action_json_objects[DSP_NUM]       = {"AccountMenuActionOne", "AccountMenuActionTwo", "AccountMenuActionThree", "AccountMenuActionFour", "AccountMenuActionFive"};
    static char *date_json_objects[DSP_NUM]         = {"AccountMenuDateOne",   "AccountMenuDateTwo",   "AccountMenuDateThree",   "AccountMenuDateFour",   "AccountMenuDateFive"};
    static char *share_amount_json_objects[DSP_NUM] = {"AccountMenuSharesOne", "AccountMenuSharesTwo", "AccountMenuSharesThree", "AccountMenuSharesFour", "AccountMenuSharesFive"};

    for (int i=0; i < DSP_NUM; i++) {

        all_name_objects[i]         = GetJSONObjectAndAddToDrawLayer(name_json_objects[i]);
        all_action_objects[i]       = GetJSONObjectAndAddToDrawLayer(action_json_objects[i]);
        all_date_objects[i]         = GetJSONObjectAndAddToDrawLayer(date_json_objects[i]);
        all_share_amount_objects[i] = GetJSONObjectAndAddToDrawLayer(share_amount_json_objects[i]);
        
    }

}

void PopulateAllStocksHistoryDisplay()
{
    
    struct Transactions *transaction = GetAllTransactions();

    char transaction_time[128];

    for (int i=0; i < DSP_NUM; i++) {

        if(transaction->shares[i]) {

            time_t time_buf = transaction->date[i];
            strftime(transaction_time, 128, "%x", localtime(&time_buf));

            SetTextContent(all_name_objects[i],         "%s", GetCompanyNameFromCompanyId(transaction->id[i]));
            SetTextContent(all_action_objects[i],       "%s", GetTransactionAction(transaction->type[i]));
            SetTextContent(all_date_objects[i],         "%s", transaction_time);
            SetTextContent(all_share_amount_objects[i], "%d", transaction->shares[i]);
        }
        
    }

    free(transaction->id);
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

        SetTextContent(selected_action_objects[i],       "%s", "");
        SetTextContent(selected_date_objects[i],         "%s", "");
        SetTextContent(selected_share_amount_objects[i], "%s", "");
        SetTextContent(selected_share_price_objects[i],  "%s", "");
        SetTextContent(selected_transaction_objects[i],  "%s", "");

    }

}

void PopulateAccountMenuCompanyScrollBox(DrawObject *object)
{

    for(int i = 0; i < GetAmountOfCompanies(); i++)
        object->scrollbox.text_content[i]  = GetCompanyNameFromCompanyId(i+1);

}

void AccountMenuCompanyScrollBoxClick(char *scroll_box_content, unsigned short int index)
{

    SetCompanyIdViewing(GetCompanyId(scroll_box_content));
    ClearAccountHistoryDisplay();
    PopulateSelectedStockHistoryDisplay(scroll_box_content);

    SetTextContent(company_name_textobject, "%s", scroll_box_content);

}

void InitalizeAccountMenuCompanyScrollbox() 
{

    DrawObject *object = CreateScrollBoxObject();

    object->x          = 2;
    object->y          = 230;
    object->width      = 288;
    object->height     = 603;
    object->asset_path = "assets/images/companyicons/StocksBox.png";

    object->scrollbox.num_items    = GetAmountOfCompanies();
    object->scrollbox.box_click    = &AccountMenuCompanyScrollBoxClick;
    object->scrollbox.text_content = malloc(sizeof(char *) * 2);

    PopulateAccountMenuCompanyScrollBox(object);
    AddObjectToDrawLayer(object);

}


void AccountDown_BCB()
{

    history_display_number += DSP_NUM;
    ClearAccountHistoryDisplay();
    PopulateSelectedStockHistoryDisplay(GetCompanyNameViewing());

}

void AccountUp_BCB()
{
    if(history_display_number >= DSP_NUM) {

        history_display_number -= DSP_NUM;
        ClearAccountHistoryDisplay();
        PopulateSelectedStockHistoryDisplay(GetCompanyNameViewing());

    }
}

void CleanAccountMenu()
{

    history_display_number = 0;

    account_menu                  = NULL;
    company_name_textobject       = NULL;
    player_money_textobject       = NULL;
    player_date_textobject        = NULL; 
    stock_price_textobject        = NULL;
    owned_stock_amount_textobject = NULL;

}