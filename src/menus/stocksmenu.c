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
#include "generalpurposemenus.h"
#include "drawlayerutils.h"
#include "textbox.h"
#include "menupersistence.h"

#include "manager.h"
#include "newsmanager.h"

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

void DisplayGraph(char *company_name, TimeSpan time_span);
void InitializeCompanyScrollBox();
void PopulateStocksScrollBox(DrawObject *object);
char *GetCurrentCompanyFromGraph();
void StocksMenusRenderLogic();
void PopulateStockStatsText(char *company_name);
void InitalizeStocksMenuText();
void InitializeTransactionMenuTextBoxes();
void InitializeBuyMenuText();
void InitializeSellMenuText();

void InitializeStocksMenu() 
{ 

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: StocksMenu could not create new draw layer"); 
        return;

    }

    stocks_menu = GetMenuWithChildsFromJsonLayer("StocksMenu");
    AddMenuWithChildsToDrawLayer(stocks_menu);
    InitalizeStocksMenuText();
    InitializeCompanyScrollBox();

    DisplayGraph(GetCompanyNameViewing(), ONE_DAY);
    PopulateStockStatsText(GetCompanyNameViewing());

    InitializeSpeedSelectObject("StocksMenu");
    DrawLayer_AddManager(NewsManager_Create(1400, 300));

}

void StocksMenusRenderLogic()
{
    
    if (player_money_textobject == NULL)
        return;

    if(stocks_menu)
    {

        SetTextContent(stock_change_textobject, "%.2f", GetCurrentStockChange(GetCompanyNameViewing()));
        SetTextContent(player_money_textobject, "%.2f", GetAccountMoney());
        SetTextContent(player_date_textobject,  "%s",   GetDate());
        SetTextContent(stock_price_textobject,  "%.2f", CurrentStockPrice(GetCompanyNameViewing()));

        if(GetCurrentStockChange(GetCompanyNameViewing()) > 0)
            SetTextColor(stock_change_textobject, 0, 79, 37, 255);
        else
            SetTextColor(stock_change_textobject, 204, 48, 56, 255);

    }

    if(buy_menu)
    {

        SetTextContent(transaction_menu_pershare_textobject,  "%.2f", CurrentStockPrice(selected_company_name));
        SetTextContent(transaction_menu_projected_textobject, "%.2f", CurrentStockPrice(selected_company_name) * atoi(GetTextFromTextBox("BuyTextBox")));

    }

    if(sell_menu)
    {

        SetTextContent(transaction_menu_pershare_textobject,  "%.2f", CurrentStockPrice(selected_company_name));
        SetTextContent(transaction_menu_projected_textobject, "%.2f", CurrentStockPrice(selected_company_name) * atoi(GetTextFromTextBox("SellTextBox")));

    }
    
}

void InitalizeStocksMenuText()
{

    company_name_textobject  = GetJSONObjectAndAddToDrawLayer("StocksMenuCompanyNameTextObject");

    company_about_textobject        = GetDrawObjectFromJsonLayer("AccountMenuAboutCompanyTextObject");
    company_about_textobject->width = 835;
    AddObjectToDrawLayer(company_about_textobject);

    player_money_textobject  = GetJSONObjectAndAddToDrawLayer("StocksMenuAccountMoneyTextObject");
    player_date_textobject   = GetJSONObjectAndAddToDrawLayer("StocksMenuAccountDateTextObject");
    stock_price_textobject   = GetJSONObjectAndAddToDrawLayer("StocksMenuStockPriceTextObject");
    stock_change_textobject  = GetJSONObjectAndAddToDrawLayer("StocksMenuPriceChangeTextObject");

    SetTextContent(stock_change_textobject,  "%.2f", GetCurrentStockChange(GetCompanyNameViewing()));
    SetTextContent(player_money_textobject,  "%.2f", GetAccountMoney());
    SetTextContent(stock_price_textobject,   "%.2f", CurrentStockPrice(GetCompanyNameViewing()));

}

void InitializeBuyMenuText()
{
    InitializeTransactionMenuTextBoxes();

    transaction_menu_company_name_textobject = GetJSONObjectAndAddToDrawLayer("BuyMenuCompanyNameTextObject");
    transaction_menu_pershare_textobject     = GetJSONObjectAndAddToDrawLayer("BuyMenuPricePerTextObject");
    transaction_menu_projected_textobject    = GetJSONObjectAndAddToDrawLayer("BuyMenuPriceAllTextObject");

    SetTextContent(transaction_menu_company_name_textobject, "%s", GetCompanyAbbreviationRef(GetCompanyId(selected_company_name)));

}

void InitializeSellMenuText()
{
    InitializeTransactionMenuTextBoxes();

    transaction_menu_company_name_textobject = GetJSONObjectAndAddToDrawLayer("SellMenuCompanyNameTextObject");
    transaction_menu_pershare_textobject     = GetJSONObjectAndAddToDrawLayer("SellMenuPricePerTextObject");
    transaction_menu_projected_textobject    = GetJSONObjectAndAddToDrawLayer("SellMenuPriceAllTextObject");
    transaction_menu_amountowned_textobject  = GetJSONObjectAndAddToDrawLayer("SellMenuAmountOwnedTextObject");

    SetTextContent(transaction_menu_company_name_textobject, "%s", GetCompanyAbbreviationRef(GetCompanyId(selected_company_name)));
    SetTextContent(transaction_menu_amountowned_textobject, "%d", GetOwnedStockAmount(selected_company_name));

}

void PopulateStockStatsText(char *company_name)
{

    SetTextContent(company_name_textobject, "%s", company_name);
    SetTextContent(company_about_textobject, "%s", GetCompanyDescriptionRef(GetCompanyId(GetCompanyNameViewing())));

}


void DisplayGraph(char *company_name, TimeSpan time_span)
{

    current_graph = GetGraphDrawObject(company_name, time_span, 961, 373);
    if (current_graph != NULL) {

        current_graph->x = 415;
        current_graph->y = 234;
        AddObjectToDrawLayer(current_graph);

    }

}

char *GetCurrentCompanyFromGraph()
{

    DrawObjectCollection *current_draw_layer_graphs = GetObjectsByType(GRAPH);
    return current_draw_layer_graphs->objects[0]->graph.company;

}

void LoadCompanyScrollBoxClick(char *scroll_box_content, unsigned short int index)
{

    char *company_name = GetCompanyName(index+1);
    SetCompanyIdViewing(GetCompanyId(company_name));

    RemoveDrawObject(current_graph);
    DisplayGraph(company_name, GetStockMenuTimeSpanNum());
    PopulateStockStatsText(company_name);

}

void InitializeCompanyScrollBox()
{

    AddObjectToDrawLayer(GetCompaniesScrollbox(2, 230, &LoadCompanyScrollBoxClick));

}

void ChangeGraphTimespan(TimeSpan time_span)
{
    SetStockMenuTimeSpanNum(time_span);
    RemoveDrawObject(current_graph);
    DisplayGraph(GetCompanyNameViewing(), time_span);
    PopulateStockStatsText(GetCompanyNameViewing());
}


void InitializeTransactionMenuTextBoxes()
{
    if(buy_menu)
    {

        DrawObject *buy_tb = CreateTextBoxObject("BuyTextBox", "", 10, TEXTBOX_ACCEPT_NUMBER_CHARACTERS);
        buy_tb->x          = 955;
        buy_tb->y          = 395;
        buy_tb->width      = 145;
        buy_tb->height     = 22;

        AddObjectToDrawLayer(buy_tb);

    }

    if(sell_menu)
    {

        DrawObject *sell_tb = CreateTextBoxObject("SellTextBox", "", 10, TEXTBOX_ACCEPT_NUMBER_CHARACTERS);
        sell_tb->x          = 955;
        sell_tb->y          = 395;
        sell_tb->width      = 145;
        sell_tb->height     = 22;

        AddObjectToDrawLayer(sell_tb);
    }

}


void SellMenu_BCB()
{
    
    if (sell_menu == NULL) {

        selected_company_name = GetCurrentCompanyFromGraph();

        CreateNewDrawLayer();

        sell_menu = GetMenuWithChildsFromJsonLayer("SellMenu");
        AddMenuWithChildsToDrawLayer(sell_menu);
        InitializeSellMenuText();

    } else {

        ClearCurrentDrawLayer();
        sell_menu = NULL;

    }

}

void BuyMenu_BCB()
{

    if (buy_menu == NULL) {

        selected_company_name = GetCurrentCompanyFromGraph();

        CreateNewDrawLayer();

        buy_menu = GetMenuWithChildsFromJsonLayer("BuyMenu");
        AddMenuWithChildsToDrawLayer(buy_menu);
        InitializeBuyMenuText();

    } else {

        ClearCurrentDrawLayer();
        buy_menu = NULL;

    }

}

void Sell_BCB()
{

    int amount_in_text_box = atoi(GetTextFromTextBox("SellTextBox"));
    if (amount_in_text_box <= 0)
        return;

    char str[50];
    float current_stock_price = CurrentStockPrice(selected_company_name);
    bool successful = AttemptToSubtractFromCurrentStock(GetCompanyNameViewing(), amount_in_text_box, CurrentStockPrice(selected_company_name));

    if (successful) {

        SellMenu_BCB();
        sprintf(str, "Sold %d of %s", amount_in_text_box, GetCompanyNameViewing());
        DisplayPopupOnDrawLayer(str, "assets/images/generalpurposemenus/popups/greenpopup.png");
        AddMoney(amount_in_text_box * current_stock_price);

    } else {

        DisplayPopupOnDrawLayer("Unable to sell stock", "assets/images/generalpurposemenus/popups/redpopup.png");

    }

}

void Buy_BCB()
{
    
    int amount_in_text_box    = atoi(GetTextFromTextBox("BuyTextBox"));
    if (amount_in_text_box <= 0)
        return;

    float current_stock_price = CurrentStockPrice(selected_company_name);

    char str[50];
    if (CanMakeTransaction(amount_in_text_box * current_stock_price)) {

        AttemptToAddFromCurrentStock(GetCompanyNameViewing(), amount_in_text_box, current_stock_price);
        BuyMenu_BCB();

        sprintf(str, "Bought %d shares of %s", amount_in_text_box, GetCompanyNameViewing());
        DisplayPopupOnDrawLayer(str, "assets/images/generalpurposemenus/popups/greenpopup.png");

        SubtractMoney(amount_in_text_box * current_stock_price);

    } 
    else {

        DisplayPopupOnDrawLayer("Unable to purchase stock", "assets/images/generalpurposemenus/popups/redpopup.png");

    }

}

void OneD_BCB()
{
    
    ChangeGraphTimespan(ONE_DAY);

}

void OneW_BCB()
{

    ChangeGraphTimespan(ONE_WEEK);

}

void OneM_BCB()
{

    ChangeGraphTimespan(ONE_MONTH);

}

void OneY_BCB()
{

    ChangeGraphTimespan(ONE_YEAR);
    
}

void All_BCB()
{

    ChangeGraphTimespan(ALL_TIME);

}

void CleanUpStocksMenu() 
{
    

    
}

void CleanStocksMenu()
{

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