#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "text.h"
#include "scrollbox.h"
#include "jsonlayer.h"
#include "drawlayers.h"

#include "log.h"
#include "graph.h"
#include "startup.h"
#include "simulation.h"
#include "popup.h"

#include "dbaccount.h"
#include "dbcompany.h"
#include "account.h"
#include "game.h"
#include "generalpurposemenus.h"
#include "drawlayerutils.h"
#include "textbox.h"
#include "menupersistence.h"
#include "transaction.h"

#include "manager.h"
#include "newsmanager.h"
#include "cardcontroller.h"

#include "controls.h"

static MenuWithChilds *stocks_menu = NULL;
static MenuWithChilds *sell_menu   = NULL;
static MenuWithChilds *buy_menu    = NULL;

static char *selected_company_name          = NULL;
static DrawObject *current_graph            = NULL;

static DrawObject *player_money_textobject  = NULL;
static DrawObject *player_date_textobject   = NULL; 

static DrawObject *company_name_textobject  = NULL;
static DrawObject *company_about_textobject = NULL;
static DrawObject *stock_price_textobject   = NULL;
static DrawObject *stock_change_textobject  = NULL;

static DrawObject *transaction_menu_company_name_textobject = NULL;
static DrawObject *transaction_menu_pershare_textobject     = NULL;
static DrawObject *transaction_menu_projected_textobject    = NULL;
static DrawObject *transaction_menu_amountowned_textobject  = NULL;

void StocksMenu_DisplayGraph(char *company_name, TimeSpan time_span);
void StocksMenu_InitCompanyScrollbox();
void PopulateStocksScrollBox(DrawObject *object);
void StocksMenu_RenderLogic();
void StocksMenu_PopulateStockInfo(char *company_name);
void StocksMenu_InitText();
void StocksMenu_InitTransactionMenuText();
void StocksMenu_BuyMenu_InitText();
void StocksMenu_SellMenu_InitText();
char *StocksMenu_CurrentCompanyFromGraph();

void StocksMenu_Init() 
{ 

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: StocksMenu could not create new draw layer"); 
        return;

    }

    stocks_menu = GetMenuWithChildsFromJsonLayer("StocksMenu");
    AddMenuWithChildsToDrawLayer(stocks_menu);
    StocksMenu_InitText();
    StocksMenu_InitCompanyScrollbox();

    StocksMenu_DisplayGraph(GetCompanyNameViewing(), ONE_DAY);
    StocksMenu_PopulateStockInfo(GetCompanyNameViewing());

    GeneralPurposeMenus_InitSpeedSelectObject("StocksMenu");
    DrawLayer_AddManager(NewsManager_Create(1400, 300));

    CardController_InitCard();

}

void StocksMenu_RenderLogic()
{
    
    if (player_money_textobject == NULL)
        return;

    if (stocks_menu) {

        CardController_CardAnimationCheck();
        CardController_CheckForNewCards();

        SetTextContent(stock_change_textobject, "%.2f", Simulation_GetStockPriceDiff(GetCompanyNameViewing()));
        SetTextContentWithCommaFormat(player_money_textobject, "%'.2f", Account_GetMoney(Account_GetPlayerId()));
        SetTextContent(player_date_textobject,  "%s",   Game_GetDate());
        SetTextContentWithCommaFormat(stock_price_textobject,  "%'.2f", Simulation_GetLastStockPrice(GetCompanyNameViewing()));

        if(Simulation_GetStockPriceDiff(GetCompanyNameViewing()) > 0)
            SetTextColor(stock_change_textobject, 0, 79, 37, 255);
        else
            SetTextColor(stock_change_textobject, 204, 48, 56, 255);

    }

    if (buy_menu) {

        SetTextContentWithCommaFormat(transaction_menu_pershare_textobject,  "%'.2f", Simulation_GetLastStockPrice(selected_company_name));
        SetTextContentWithCommaFormat(transaction_menu_projected_textobject, "%'.2f", Simulation_GetLastStockPrice(selected_company_name) * atoi(GetTextFromTextBox("BuyTextBox")));

    }

    if (sell_menu) {

        SetTextContentWithCommaFormat(transaction_menu_pershare_textobject,  "%'.2f", Simulation_GetLastStockPrice(selected_company_name));
        SetTextContentWithCommaFormat(transaction_menu_projected_textobject, "%'.2f", Simulation_GetLastStockPrice(selected_company_name) * atoi(GetTextFromTextBox("SellTextBox")));

    }
    
}

void StocksMenu_InitText()
{

    company_name_textobject  = GetJSONObjectAndAddToDrawLayer("StocksMenuCompanyNameTextObject");

    company_about_textobject        = GetDrawObjectFromJsonLayer("AccountMenuAboutCompanyTextObject");
    company_about_textobject->width = 835;
    AddObjectToDrawLayer(company_about_textobject);

    player_money_textobject  = GetJSONObjectAndAddToDrawLayer("StocksMenuAccountMoneyTextObject");
    player_date_textobject   = GetJSONObjectAndAddToDrawLayer("StocksMenuAccountDateTextObject");
    stock_price_textobject   = GetJSONObjectAndAddToDrawLayer("StocksMenuStockPriceTextObject");
    stock_change_textobject  = GetJSONObjectAndAddToDrawLayer("StocksMenuPriceChangeTextObject");

    SetTextContentWithCommaFormat(stock_change_textobject,  "%'.2f", Simulation_GetStockPriceDiff(GetCompanyNameViewing()));
    SetTextContentWithCommaFormat(player_money_textobject,  "%'.2f", Account_GetMoney(Account_GetPlayerId()));
    SetTextContentWithCommaFormat(stock_price_textobject,   "%'.2f", Simulation_GetLastStockPrice(GetCompanyNameViewing()));

}

void StocksMenu_BuyMenu_InitText()
{
    StocksMenu_InitTransactionMenuText();

    transaction_menu_company_name_textobject = GetJSONObjectAndAddToDrawLayer("BuyMenuCompanyNameTextObject");
    transaction_menu_pershare_textobject     = GetJSONObjectAndAddToDrawLayer("BuyMenuPricePerTextObject");
    transaction_menu_projected_textobject    = GetJSONObjectAndAddToDrawLayer("BuyMenuPriceAllTextObject");

    SetTextContent(transaction_menu_company_name_textobject, "%s", GetCompanyAbbreviationRef(GetCompanyId(selected_company_name)));

}

void StocksMenu_SellMenu_InitText()
{
    StocksMenu_InitTransactionMenuText();

    transaction_menu_company_name_textobject = GetJSONObjectAndAddToDrawLayer("SellMenuCompanyNameTextObject");
    transaction_menu_pershare_textobject     = GetJSONObjectAndAddToDrawLayer("SellMenuPricePerTextObject");
    transaction_menu_projected_textobject    = GetJSONObjectAndAddToDrawLayer("SellMenuPriceAllTextObject");
    transaction_menu_amountowned_textobject  = GetJSONObjectAndAddToDrawLayer("SellMenuAmountOwnedTextObject");

    uint32_t company_id = GetCompanyId(selected_company_name);
    SetTextContent(transaction_menu_company_name_textobject, "%s", GetCompanyAbbreviationRef(company_id));
    SetTextContent(transaction_menu_amountowned_textobject, "%d", dbaccount_get_owned_stock_amount(Account_GetPlayerId(), company_id));

}

void StocksMenu_PopulateStockInfo(char *company_name)
{

    SetTextContent(company_name_textobject, "%s", company_name);
    SetTextContent(company_about_textobject, "%s", GetCompanyDescriptionRef(GetCompanyId(GetCompanyNameViewing())));

}


void StocksMenu_DisplayGraph(char *company_name, TimeSpan time_span)
{

    current_graph = Graph_GetGraphDrawObject(company_name, time_span, 961, 373);
    if (current_graph != NULL) {

        current_graph->x = 415;
        current_graph->y = 234;
        AddObjectToDrawLayer(current_graph);

    }

}

char *StocksMenu_CurrentCompanyFromGraph()
{

    DrawObjectCollection *current_draw_layer_graphs = GetObjectsByType(GRAPH);
    return current_draw_layer_graphs->objects[0]->graph.company;

}

void StocksMenu_CompanyScrollboxClick(char *scroll_box_content, unsigned short int index)
{

    char *company_name = GetCompanyName(index+1);
    SetCompanyIdViewing(GetCompanyId(company_name));

    RemoveDrawObject(current_graph);
    StocksMenu_DisplayGraph(company_name, GetStockMenuTimeSpanNum());
    StocksMenu_PopulateStockInfo(company_name);

    selected_company_name = StocksMenu_CurrentCompanyFromGraph();

}

void StocksMenu_InitCompanyScrollbox()
{

    AddObjectToDrawLayer(CreateCompaniesScrollbox(2, 230, &StocksMenu_CompanyScrollboxClick));

}

void StocksMenu_ChangeGraphTimespan(TimeSpan time_span)
{

    SetStockMenuTimeSpanNum(time_span);
    RemoveDrawObject(current_graph);
    StocksMenu_DisplayGraph(GetCompanyNameViewing(), time_span);
    StocksMenu_PopulateStockInfo(GetCompanyNameViewing());
    
}


void StocksMenu_InitTransactionMenuText()
{
    if(buy_menu) {

        DrawObject *buy_tb = CreateTextBoxObject("BuyTextBox", "", 10, TEXTBOX_ACCEPT_NUMBER_CHARACTERS);
        buy_tb->x          = 955;
        buy_tb->y          = 395;
        buy_tb->width      = 145;
        buy_tb->height     = 22;

        AddObjectToDrawLayer(buy_tb);

    }

    if(sell_menu) {

        DrawObject *sell_tb = CreateTextBoxObject("SellTextBox", "", 10, TEXTBOX_ACCEPT_NUMBER_CHARACTERS);
        sell_tb->x          = 955;
        sell_tb->y          = 395;
        sell_tb->width      = 145;
        sell_tb->height     = 22;

        AddObjectToDrawLayer(sell_tb);
    }

}


void StocksMenu_SellMenu_BCB()
{
    
    if (sell_menu == NULL) {

        selected_company_name = StocksMenu_CurrentCompanyFromGraph();

        CreateNewDrawLayer();

        sell_menu = GetMenuWithChildsFromJsonLayer("SellMenu");
        AddMenuWithChildsToDrawLayer(sell_menu);
        StocksMenu_SellMenu_InitText();

    } else {

        ClearCurrentDrawLayer();
        sell_menu = NULL;

    }

}

void StocksMenu_BuyMenu_BCB()
{

    if (buy_menu == NULL) {

        selected_company_name = StocksMenu_CurrentCompanyFromGraph();

        CreateNewDrawLayer();

        buy_menu = GetMenuWithChildsFromJsonLayer("BuyMenu");
        AddMenuWithChildsToDrawLayer(buy_menu);
        StocksMenu_BuyMenu_InitText();

    } else {

        ClearCurrentDrawLayer();
        buy_menu = NULL;

    }

}

void StocksMenu_SellMenu_Sell_BCB()
{

    int amount_in_text_box = atoi(GetTextFromTextBox("SellTextBox"));
    if (amount_in_text_box <= 0)
        return;

    char *company_viewing = GetCompanyNameViewing();
    uint32_t company_id   = GetCompanyId(company_viewing);
    uint32_t player_id    = Account_GetPlayerId();

    if (transaction_sell_stock_amount(player_id, company_id, amount_in_text_box)) {

        StocksMenu_SellMenu_BCB();

        char str[50];
        sprintf(str, "Sold %d of %s", amount_in_text_box, company_viewing);
        DisplayPopupOnDrawLayer(str, "assets/images/generalpurposemenus/popups/greenpopup.png");

    } else {

        DisplayPopupOnDrawLayer("Unable to sell stock", "assets/images/generalpurposemenus/popups/redpopup.png");

    }

}

void StocksMenu_BuyMenu_Buy_BCB()
{
    
    int amount_in_text_box = atoi(GetTextFromTextBox("BuyTextBox"));
    if (amount_in_text_box <= 0)
        return;

    uint32_t player_id    = Account_GetPlayerId();
    char *company_viewing = GetCompanyNameViewing();
    uint32_t company_id   = GetCompanyId(company_viewing);

    if (transaction_purchase_stock_amount(player_id, company_id, amount_in_text_box)) {

        StocksMenu_BuyMenu_BCB();
        char str[50];
        sprintf(str, "Bought %d shares of %s", amount_in_text_box, company_viewing);
        DisplayPopupOnDrawLayer(str, "assets/images/generalpurposemenus/popups/greenpopup.png");

    } else {

        DisplayPopupOnDrawLayer("Unable to purchase stock", "assets/images/generalpurposemenus/popups/redpopup.png");

    }

}

void OneD_BCB()
{
    
    StocksMenu_ChangeGraphTimespan(ONE_DAY);

}

void OneW_BCB()
{

    StocksMenu_ChangeGraphTimespan(ONE_WEEK);

}

void OneM_BCB()
{

    StocksMenu_ChangeGraphTimespan(ONE_MONTH);

}

void OneY_BCB()
{

    StocksMenu_ChangeGraphTimespan(ONE_YEAR);
    
}

void All_BCB()
{

    StocksMenu_ChangeGraphTimespan(ALL_TIME);

}

void StocksMenu_Clean()
{

    CardController_Clean();

    selected_company_name    = NULL;
    current_graph            = NULL;

    player_money_textobject  = NULL;
    player_date_textobject   = NULL; 

    company_name_textobject  = NULL;
    company_about_textobject = NULL;
    stock_price_textobject   = NULL;
    stock_change_textobject  = NULL;

    transaction_menu_company_name_textobject = NULL;
    transaction_menu_pershare_textobject     = NULL;
    transaction_menu_projected_textobject    = NULL;
    transaction_menu_amountowned_textobject  = NULL;

    stocks_menu = NULL;
    sell_menu   = NULL;
    buy_menu    = NULL;

}