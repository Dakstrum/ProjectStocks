#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"
#include "dbcompany.h"
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
static DrawObject *company_about_textobject      = NULL;
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
    InitializeSpeedSelectObject("AccountMenu");

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

    char *company_viewing = GetCompanyNameViewing();
    
    SetTextContent(player_money_textobject,       "%.2f", GetAccountMoney());
    SetTextContent(player_date_textobject,        "%s",   GetDate());
    SetTextContent(stock_price_textobject,        "%.2f", CurrentStockPrice(company_viewing));
    SetTextContent(owned_stock_amount_textobject, "%d",   GetOwnedStockAmount(company_viewing));

}

void InitalizeAccountMenuText()
{

    company_name_textobject       = GetJSONObjectAndAddToDrawLayer("AccountMenuCompanyNameTextObject");
    company_about_textobject      = GetJSONObjectAndAddToDrawLayer("AccountMenuAboutCompanyTextObject");
    player_money_textobject       = GetJSONObjectAndAddToDrawLayer("AccountMenuAccountMoneyTextObject");
    player_date_textobject        = GetJSONObjectAndAddToDrawLayer("AccountMenuAccountDateTextObject");
    stock_price_textobject        = GetJSONObjectAndAddToDrawLayer("AccountMenuStockPriceTextObject");
    owned_stock_amount_textobject = GetJSONObjectAndAddToDrawLayer("AccountMenuAmountOwnedTextObject");

    SetTextContent(company_name_textobject, "%s", GetCompanyNameViewing());
    SetTextContent(company_about_textobject, "%s", GetCompanyDescriptionRef(GetCompanyId(GetCompanyNameViewing())));

}

void InitializeSelectedStockHistoryDisplay()
{

    static char *action_json_objects[DSP_NUM]       = {"AccountMenuAction1TextObject",      "AccountMenuAction2TextObject",      "AccountMenuAction3TextObject",      "AccountMenuAction4TextObject",      "AccountMenuAction5TextObject"};
    static char *date_json_objects[DSP_NUM]         = {"AccountMenuDate1TextObject",        "AccountMenuDate2TextObject",        "AccountMenuDate3TextObject",        "AccountMenuDate4TextObject",        "AccountMenuDate5TextObject"};
    static char *share_amount_json_objects[DSP_NUM] = {"AccountMenuShares1TextObject",      "AccountMenuShares2TextObject",      "AccountMenuShares3TextObject",      "AccountMenuShares4TextObject",      "AccountMenuShares5TextObject"};
    static char *share_price_json_objects[DSP_NUM]  = {"AccountMenuPerShare1TextObject",    "AccountMenuPerShare2TextObject",    "AccountMenuPerShare3TextObject",    "AccountMenuPerShare4TextObject",    "AccountMenuPerShare5TextObject"};
    static char *transaction_json_objects[DSP_NUM]  = {"AccountMenuTransaction1TextObject", "AccountMenuTransaction2TextObject", "AccountMenuTransaction3TextObject", "AccountMenuTransaction4TextObject", "AccountMenuTransaction5TextObject"};

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
    
    struct Transactions *transactions = GetCompanyTransactions(company);

    char transaction_time[128];

    for (int i=0; i < DSP_NUM; i++) {

        if(transactions->shares[history_display_number + i]) {

            time_t time_buf = transactions->date[history_display_number + i];
            strftime(transaction_time, 128, "%x", localtime(&time_buf));

            SetTextContent(selected_action_objects[i],       "%s",   GetTransactionAction(transactions->type[history_display_number + i]));
            SetTextContent(selected_date_objects[i],         "%s",   transaction_time);
            SetTextContent(selected_share_amount_objects[i], "%d",   transactions->shares[history_display_number + i]);
            SetTextContent(selected_share_price_objects[i],  "%.2f", transactions->pershare[history_display_number + i]);
            SetTextContent(selected_transaction_objects[i],  "%.2f", transactions->transaction[history_display_number + i]);

        }
        
    }
    FreeTransactions(transactions);
    
}

void InitializeAllStocksHistoryDisplay()
{

    static char *name_json_objects[DSP_NUM]         = {"AccountMenuName11TextObject",   "AccountMenuName22TextObject",   "AccountMenuName33TextObject",   "AccountMenuName44TextObject",   "AccountMenuName55TextObject"};
    static char *action_json_objects[DSP_NUM]       = {"AccountMenuAction11TextObject", "AccountMenuAction22TextObject", "AccountMenuAction33TextObject", "AccountMenuAction44TextObject", "AccountMenuAction55TextObject"};
    static char *date_json_objects[DSP_NUM]         = {"AccountMenuDate11TextObject",   "AccountMenuDate22TextObject",   "AccountMenuDate33TextObject",   "AccountMenuDate44TextObject",   "AccountMenuDate55TextObject"};
    static char *share_amount_json_objects[DSP_NUM] = {"AccountMenuShares11TextObject", "AccountMenuShares22TextObject", "AccountMenuShares33TextObject", "AccountMenuShares44TextObject", "AccountMenuShares55TextObject"};

    for (int i=0; i < DSP_NUM; i++) {

        all_name_objects[i]         = GetJSONObjectAndAddToDrawLayer(name_json_objects[i]);
        all_action_objects[i]       = GetJSONObjectAndAddToDrawLayer(action_json_objects[i]);
        all_date_objects[i]         = GetJSONObjectAndAddToDrawLayer(date_json_objects[i]);
        all_share_amount_objects[i] = GetJSONObjectAndAddToDrawLayer(share_amount_json_objects[i]);
        
    }

}

void PopulateAllStocksHistoryDisplay()
{
    
    struct Transactions *transactions = GetAllTransactions();

    char transaction_time[128];

    for (int i=0; i < DSP_NUM; i++) {

        if(transactions->shares[i]) {

            time_t time_buf = transactions->date[i];
            strftime(transaction_time, 128, "%x", localtime(&time_buf));

            SetTextContent(all_name_objects[i],         "%s", GetCompanyName(transactions->company_id[i]));
            SetTextContent(all_action_objects[i],       "%s", GetTransactionAction(transactions->type[i]));
            SetTextContent(all_date_objects[i],         "%s", transaction_time);
            SetTextContent(all_share_amount_objects[i], "%d", transactions->shares[i]);
        }
        
    }

    FreeTransactions(transactions);
    
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

    for(int i = 0; i < GetNumCompanies(); i++)
        object->scrollbox.text_content[i]  = GetCompanyAbbreviation(i+1);

}

void AccountMenuCompanyScrollBoxClick(char *scroll_box_content, unsigned short int index)
{

    char *company_name = GetCompanyName(index+1);
    SetCompanyIdViewing(GetCompanyId(company_name));
    ClearAccountHistoryDisplay();
    PopulateSelectedStockHistoryDisplay(company_name);

    SetTextContent(company_name_textobject, "%s", company_name);
    SetTextContent(company_about_textobject, "%s", GetCompanyDescriptionRef(GetCompanyId(GetCompanyNameViewing())));

}

void InitalizeAccountMenuCompanyScrollbox() 
{

    DrawObject *object = CreateScrollBoxObject();

    object->x          = 2;
    object->y          = 230;
    object->width      = 288;
    object->height     = 603;
    object->asset_path = "assets/images/companyicons/StocksBox.png";

    object->scrollbox.num_items    = GetNumCompanies();
    object->scrollbox.box_click    = &AccountMenuCompanyScrollBoxClick;
    object->scrollbox.text_content = malloc(sizeof(char *) * GetNumCompanies());

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