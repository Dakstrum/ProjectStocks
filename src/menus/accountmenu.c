#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "vector.h"

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"
#include "dbcompany.h"
#include "dbaccount.h"
#include "text.h"
#include "scrollbox.h"
#include "account.h"
#include "game.h"
#include "simulation.h"
#include "generalpurposemenus.h"
#include "drawlayerutils.h"
#include "menupersistence.h"

#define DSP_NUM 5

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

static DrawObject *historydisplay_pages_textobject       = NULL;
static DrawObject *historydisplay_currentpage_textobject = NULL;
static DrawObject *networth_textobject                   = NULL;

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
void SetHistoryDisplayPageNumber(unsigned int num_transactions);

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

}

void AccountMenuRenderLogic()
{

    if (player_money_textobject == NULL)
        return;

    char *company_viewing = GetCompanyNameViewing();
    
    SetTextContent(player_money_textobject,       "%.2f", Account_GetMoney());
    SetTextContent(player_date_textobject,        "%s",   Game_GetDate());
    SetTextContent(stock_price_textobject,        "%.2f", CurrentStockPrice(company_viewing));
    SetTextContent(owned_stock_amount_textobject, "%d",   GetOwnedStockAmount(Account_GetPlayerId(), company_viewing));
    SetTextContent(networth_textobject,           "%.2f", GetAccountNetWorth(Account_GetPlayerId()));

}

void InitalizeAccountMenuText()
{

    company_name_textobject         = GetJSONObjectAndAddToDrawLayer("AccountMenuCompanyNameTextObject");
    company_about_textobject        = GetDrawObjectFromJsonLayer("AccountMenuAboutCompanyTextObject");
    company_about_textobject->width = 835;
    AddObjectToDrawLayer(company_about_textobject);
    

    player_money_textobject               = GetJSONObjectAndAddToDrawLayer("AccountMenuAccountMoneyTextObject");
    player_date_textobject                = GetJSONObjectAndAddToDrawLayer("AccountMenuAccountDateTextObject");
    stock_price_textobject                = GetJSONObjectAndAddToDrawLayer("AccountMenuStockPriceTextObject");
    owned_stock_amount_textobject         = GetJSONObjectAndAddToDrawLayer("AccountMenuAmountOwnedTextObject");
    historydisplay_pages_textobject       = GetJSONObjectAndAddToDrawLayer("AccountMenupageTextObject");
    historydisplay_currentpage_textobject = GetJSONObjectAndAddToDrawLayer("AccountMenucurrentpageTextObject");
    networth_textobject                   = GetJSONObjectAndAddToDrawLayer("AccountMenunetworthTextObject");

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
    

    Vector *transactions = GetCompanyTransactions(Account_GetPlayerId(), company);
    Transaction *temp    = transactions->elements;

    SetHistoryDisplayPageNumber(transactions->num_elements);
    char transaction_time[128];

    for (int i = 0; i < DSP_NUM; i++) {

        uint32_t off_idx = GetAccountHistoryDisplayNum() + i;
        if (off_idx >= transactions->num_elements)
            break;

        time_t time_buf = temp[off_idx].transaction_date;
        strftime(transaction_time, 128, "%x", localtime(&time_buf));

        SetTextContent(selected_action_objects[i],       "%s",   GetTransactionAction(temp[off_idx].type));
        SetTextContent(selected_date_objects[i],         "%s",   transaction_time);
        SetTextContent(selected_share_amount_objects[i], "%d",   temp[off_idx].shares_exchanged);
        SetTextContent(selected_share_price_objects[i],  "%.2f", temp[off_idx].price_per_share);
        SetTextContent(selected_transaction_objects[i],  "%.2f", temp[off_idx].transaction_amount);

        
    }

    Vector_Delete(transactions);
    
}

void SetHistoryDisplayPageNumber(unsigned int num_transactions)
{

    int amount_of_pages = ceil((float)num_transactions  / 5);
    int current_page    = GetAccountHistoryDisplayNum() / 5;

    SetTextContent(historydisplay_pages_textobject,        "%d", amount_of_pages);
    SetTextContent(historydisplay_currentpage_textobject,  "%d", current_page + 1);

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
    
    Vector *transactions = GetAllTransactions(Account_GetPlayerId());
    char transaction_time[128];

    Transaction *temp = transactions->elements;
    for (int i = 0; i < DSP_NUM; i++) {

        if(i < transactions->num_elements) {

            time_t time_buf = temp[i].transaction_date;
            strftime(transaction_time, 128, "%x", localtime(&time_buf));

            SetTextContent(all_name_objects[i],         "%s", GetCompanyAbbreviation(temp[i].company_id));
            SetTextContent(all_action_objects[i],       "%s", GetTransactionAction(temp[i].type));
            SetTextContent(all_date_objects[i],         "%s", transaction_time);
            SetTextContent(all_share_amount_objects[i], "%d", temp[i].shares_exchanged);

        }
        
    }
    Vector_Delete(transactions);

}

char *GetTransactionAction(TransactionType type)
{

    if(type == BUY)
        return "Buy";

    if(type == SELL)
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

void AccountMenuCompanyScrollBoxClick(char *scroll_box_content, unsigned short int index)
{

    SetAccountHistoryDisplayNum(0);
    char *company_name = GetCompanyName(index+1);
    SetCompanyIdViewing(GetCompanyId(company_name));
    ClearAccountHistoryDisplay();
    PopulateSelectedStockHistoryDisplay(company_name);
    

    SetTextContent(company_name_textobject, "%s", company_name);
    SetTextContent(company_about_textobject, "%s", GetCompanyDescriptionRef(GetCompanyId(GetCompanyNameViewing())));
    SetTextContent(historydisplay_currentpage_textobject,  "%d", 1);



}

void InitalizeAccountMenuCompanyScrollbox() 
{

    AddObjectToDrawLayer(CreateCompaniesScrollbox(2, 230, &AccountMenuCompanyScrollBoxClick));

}

void AccountDown_BCB()
{

    SetAccountHistoryDisplayNum(GetAccountHistoryDisplayNum() + DSP_NUM);
    ClearAccountHistoryDisplay();
    PopulateSelectedStockHistoryDisplay(GetCompanyNameViewing());

}

void AccountUp_BCB()
{
    if(GetAccountHistoryDisplayNum() >= DSP_NUM) {

        SetAccountHistoryDisplayNum(GetAccountHistoryDisplayNum() - DSP_NUM);
        ClearAccountHistoryDisplay();
        PopulateSelectedStockHistoryDisplay(GetCompanyNameViewing());

    }
}

void CleanAccountMenu()
{

    SetAccountHistoryDisplayNum(0);

    account_menu                    = NULL;
    company_name_textobject         = NULL;
    player_money_textobject         = NULL;
    player_date_textobject          = NULL; 
    stock_price_textobject          = NULL;
    owned_stock_amount_textobject   = NULL;
    historydisplay_pages_textobject = NULL;
    networth_textobject = NULL;

}