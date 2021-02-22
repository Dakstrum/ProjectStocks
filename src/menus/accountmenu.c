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

char* AccountMenu_GetTransactionActionType(TransactionType type);

void AccountMenu_InitScrollbox();
void AccountMenu_PopulateSelectedCompanyHistoryDisplay(char* company);

void AccountMenu_InitAllStocksHistoryDisplay();
void AccountMenu_PopulateAllStocksHistoryDisplay();

void AccountMenu_InitSelectedCompanyHistoryDisplay();
void AccountMenu_RenderLogic();
void AccountMenu_InitText();
void AccountMenu_SetHistoryDisplayPageNumber(unsigned int num_transactions);

void AccountMenu_Init() 
{

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: account Menu could not create new draw layer");
        return;
    }

    account_menu = GetJSONMenuAndAddToDrawLayer("AccountMenu");

    AccountMenu_InitScrollbox();
    AccountMenu_InitText();
    GeneralPurposeMenus_InitSpeedSelectObject("AccountMenu");

    AccountMenu_InitSelectedCompanyHistoryDisplay();
    AccountMenu_PopulateSelectedCompanyHistoryDisplay(GetCompanyNameViewing());

    AccountMenu_InitAllStocksHistoryDisplay();
    AccountMenu_PopulateAllStocksHistoryDisplay();

}

void AccountMenu_RenderLogic()
{

    if (player_money_textobject == NULL)
        return;

    char *company_viewing = GetCompanyNameViewing();
    uint32_t company_id   = GetCompanyId(company_viewing);
    
    SetTextContent(player_money_textobject,       "%.2f", Account_GetMoney(Account_GetPlayerId()));
    SetTextContent(player_date_textobject,        "%s",   Game_GetDate());
    SetTextContent(stock_price_textobject,        "%.2f", Simulation_GetLastStockPrice(company_viewing));
    SetTextContent(owned_stock_amount_textobject, "%d",   GetOwnedStockAmount(Account_GetPlayerId(), company_id));
    SetTextContent(networth_textobject,           "%.2f", GetAccountNetWorth(Account_GetPlayerId()));

}

void AccountMenu_InitText()
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

void AccountMenu_InitSelectedCompanyHistoryDisplay()
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

void AccountMenu_PopulateSelectedCompanyHistoryDisplay(char* company)
{
    

    Vector *transactions = GetCompanyTransactions(Account_GetPlayerId(), company);
    Transaction *temp    = transactions->elements;

    AccountMenu_SetHistoryDisplayPageNumber(transactions->num_elements);
    char transaction_time[128];

    for (int i = 0; i < DSP_NUM; i++) {

        uint32_t off_idx = GetAccountHistoryDisplayNum() + i;
        if (off_idx >= transactions->num_elements)
            break;

        time_t time_buf = temp[off_idx].transaction_date;
        strftime(transaction_time, 128, "%x", localtime(&time_buf));

        SetTextContent(selected_action_objects[i],       "%s",   AccountMenu_GetTransactionActionType(temp[off_idx].type));
        SetTextContent(selected_date_objects[i],         "%s",   transaction_time);
        SetTextContent(selected_share_amount_objects[i], "%d",   temp[off_idx].shares_exchanged);
        SetTextContent(selected_share_price_objects[i],  "%.2f", temp[off_idx].price_per_share);
        SetTextContent(selected_transaction_objects[i],  "%.2f", temp[off_idx].transaction_amount);

        
    }

    Vector_Delete(transactions);
    
}

void AccountMenu_SetHistoryDisplayPageNumber(unsigned int num_transactions)
{

    int amount_of_pages = ceil((float)num_transactions  / 5);
    int current_page    = GetAccountHistoryDisplayNum() / 5;

    SetTextContent(historydisplay_pages_textobject,        "%d", amount_of_pages);
    SetTextContent(historydisplay_currentpage_textobject,  "%d", current_page + 1);

}

void AccountMenu_InitAllStocksHistoryDisplay()
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

void AccountMenu_PopulateAllStocksHistoryDisplay()
{
    
    Vector *transactions = dbaccount_get_player_transactions(Account_GetPlayerId());
    char transaction_time[128];

    Transaction *temp = transactions->elements;
    for (int i = 0; i < DSP_NUM; i++) {

        if(i < transactions->num_elements) {

            time_t time_buf = temp[i].transaction_date;
            strftime(transaction_time, 128, "%x", localtime(&time_buf));

            SetTextContent(all_name_objects[i],         "%s", GetCompanyAbbreviation(temp[i].company_id));
            SetTextContent(all_action_objects[i],       "%s", AccountMenu_GetTransactionActionType(temp[i].type));
            SetTextContent(all_date_objects[i],         "%s", transaction_time);
            SetTextContent(all_share_amount_objects[i], "%d", temp[i].shares_exchanged);

        }
        
    }
    Vector_Delete(transactions);

}

char *AccountMenu_GetTransactionActionType(TransactionType type)
{

    if(type == BUY)
        return "Buy";

    if(type == SELL)
        return "Sell";

    return "error";
}

void AccountMenu_ClearSelectedCompanyHistoryDisplay()
{

    for (int i=0; i < DSP_NUM; i++) {

        SetTextContent(selected_action_objects[i],       "%s", "");
        SetTextContent(selected_date_objects[i],         "%s", "");
        SetTextContent(selected_share_amount_objects[i], "%s", "");
        SetTextContent(selected_share_price_objects[i],  "%s", "");
        SetTextContent(selected_transaction_objects[i],  "%s", "");

    }

}

void AccountMenu_ScrollBoxClick(char *scroll_box_content, unsigned short int index)
{

    SetAccountHistoryDisplayNum(0);
    char *company_name = GetCompanyName(index+1);
    SetCompanyIdViewing(GetCompanyId(company_name));
    AccountMenu_ClearSelectedCompanyHistoryDisplay();
    AccountMenu_PopulateSelectedCompanyHistoryDisplay(company_name);
    

    SetTextContent(company_name_textobject, "%s", company_name);
    SetTextContent(company_about_textobject, "%s", GetCompanyDescriptionRef(GetCompanyId(GetCompanyNameViewing())));
    SetTextContent(historydisplay_currentpage_textobject,  "%d", 1);



}

void AccountMenu_InitScrollbox() 
{

    AddObjectToDrawLayer(CreateCompaniesScrollbox(2, 230, &AccountMenu_ScrollBoxClick));

}

void AccountMenu_Down_CB()
{

    SetAccountHistoryDisplayNum(GetAccountHistoryDisplayNum() + DSP_NUM);
    AccountMenu_ClearSelectedCompanyHistoryDisplay();
    AccountMenu_PopulateSelectedCompanyHistoryDisplay(GetCompanyNameViewing());

}

void AccountMenu_Up_CB()
{
    if(GetAccountHistoryDisplayNum() >= DSP_NUM) {

        SetAccountHistoryDisplayNum(GetAccountHistoryDisplayNum() - DSP_NUM);
        AccountMenu_ClearSelectedCompanyHistoryDisplay();
        AccountMenu_PopulateSelectedCompanyHistoryDisplay(GetCompanyNameViewing());

    }
}

void AccountMenu_Clear()
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