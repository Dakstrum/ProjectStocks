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
#include "account.h"
#include "generalpurposemenus.h"
#include "drawlayerutils.h"

static MenuWithChilds *stocks_menu           = NULL;
static MenuWithChilds *sell_transaction_menu = NULL;
static MenuWithChilds *buy_transaction_menu  = NULL;

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

void DisplayGraph(char *company_name, TimeSpan time_span);
void InitializeCompanyScrollBox();
void PopulateStocksScrollBox(DrawObject *object);
char *GetCurrentCompanyFromGraph();
void StocksMenusRenderLogic();
void PopulateStockStatsText(char *company_name);
void InitalizeStocksMenuText();
void InitializeTransactionMenuText();

void InitializeStocksMenu() 
{ 

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: StocksMenu could not create new draw layer");
        return;

    }

    stocks_menu = GetJSONMenuAndAddToDrawLayer("StocksMenu");
    
    InitalizeStocksMenuText();
    InitializeCompanyScrollBox();

    DisplayGraph(GetCompanyNameViewing(), ONE_DAY);
    PopulateStockStatsText(GetCompanyNameViewing());

    InitializeSpeedSelectObject();

}

void StocksMenusRenderLogic()
{
    if (player_money_textobject == NULL)
        return;

    if(stocks_menu)
    {

        SetTextContent(stock_change_textobject, "%.2f", GetCurrentStockChange(GetCompanyNameViewing())); //Mem leak
        SetTextContent(player_money_textobject, "%.2f", GetAccountMoney());
        SetTextContent(player_date_textobject,  "%s",   GetDate());
        SetTextContent(stock_price_textobject,  "%.2f", CurrentStockPrice(GetCompanyNameViewing())); //Mem leak

    }

    if(buy_transaction_menu)
    {

        SetTextContent(transaction_menu_pershare_textobject,  "%.2f", CurrentStockPrice(selected_company_name));
        SetTextContent(transaction_menu_projected_textobject, "%.2f", CurrentStockPrice(selected_company_name) * atoi(GetTextFromTextBox("BuyTextBox")));

    }

    if(sell_transaction_menu)
    {

        SetTextContent(transaction_menu_pershare_textobject,  "%.2f", CurrentStockPrice(selected_company_name));
        SetTextContent(transaction_menu_projected_textobject, "%.2f", CurrentStockPrice(selected_company_name) * atoi(GetTextFromTextBox("SellTextBox")));

    }

}


void InitalizeStocksMenuText()
{

    company_name_textobject  = GetJSONObjectAndAddToDrawLayer("StocksMenuChangingCompanyNameText");
    company_about_textobject = GetJSONObjectAndAddToDrawLayer("StocksMenuChangingAboutText");
    player_money_textobject  = GetJSONObjectAndAddToDrawLayer("StocksMenuAccountMoneyText");
    player_date_textobject   = GetJSONObjectAndAddToDrawLayer("StocksMenuAccountDateText");
    stock_price_textobject   = GetJSONObjectAndAddToDrawLayer("StocksMenuCurrentStockPriceText");
    stock_change_textobject  = GetJSONObjectAndAddToDrawLayer("StocksMenuStockChangeText");

    SetTextContent(stock_change_textobject, "%.2f", GetCurrentStockChange(GetCompanyNameViewing()));
    SetTextContent(player_money_textobject, "%.2f", GetAccountMoney());
    SetTextContent(stock_price_textobject,  "%.2f", CurrentStockPrice(GetCompanyNameViewing()));

}

void InitializeTransactionMenuText()
{

    transaction_menu_company_name_textobject = GetJSONObjectAndAddToDrawLayer("TransactionMenuCompanyNameText");
    transaction_menu_pershare_textobject     = GetJSONObjectAndAddToDrawLayer("TransactionMenuPerShareText");
    transaction_menu_projected_textobject    = GetJSONObjectAndAddToDrawLayer("TransactionMenuProjectedCostText");

    SetTextContent(transaction_menu_company_name_textobject, "%s", selected_company_name);

}

void DisplayBuyPopUp(int stock_amount, char *stock_name)
{

    DrawObject *object = CreateNewPopup();
    char str[50];
    sprintf(str, "Bought %d of %s", stock_amount, stock_name);
    SetPopupText(object, str); //Mem leak
    AddObjectToDrawLayer(object);

}

void DisplaySellPopUp(int stock_amount, char *stock_name)
{

    DrawObject *object = CreateNewPopup();
    char str[50];
    sprintf(str, "Sold %d of %s", stock_amount, stock_name);
    SetPopupText(object, str); //Mem leak
    AddObjectToDrawLayer(object);

}

void PopulateStockStatsText(char *company_name)
{

    SetTextContent(company_name_textobject, "%s", company_name);
    SetTextContent(company_about_textobject, "Dynamic Description of a company");
    StocksMenusRenderLogic();

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

    DrawObjectTypeCollection *current_draw_layer_graphs = GetObjectsByType(GRAPH);
    return current_draw_layer_graphs->objects[0]->graph.company;

}

void LoadCompanyScrollBoxClick(char *scroll_box_content, unsigned short int index)
{

    SetCompanyIdViewing(GetCompanyId(scroll_box_content));

    RemoveDrawObject(current_graph);
    DisplayGraph(scroll_box_content, ONE_DAY);
    PopulateStockStatsText(scroll_box_content);

}

void InitializeCompanyScrollBox()
{

    DrawObject *object = CreateScrollBoxObject();

    object->x          = 2;
    object->y          = 230;
    object->width      = 288;
    object->height     = 603;
    object->asset_path = "assets/images/companyicons/StocksBox.png";

    object->scrollbox.num_items    = GetAmountOfCompanies();
    object->scrollbox.box_click    = &LoadCompanyScrollBoxClick;
    object->scrollbox.text_content = malloc(sizeof(char *) * 2);

    PopulateStocksScrollBox(object);
    AddObjectToDrawLayer(object);

}

void PopulateStocksScrollBox(DrawObject *object)
{

    for(int i = 0; i < GetAmountOfCompanies(); i++)
        object->scrollbox.text_content[i]  = GetStockNameFromStockId(i+1);

}

void ChangeGraphTimespan(TimeSpan time_span)
{

    RemoveDrawObject(current_graph);
    DisplayGraph(GetCompanyNameViewing(), time_span);
    PopulateStockStatsText(GetCompanyNameViewing());

}

void SellMenu_BCB()
{

    if (sell_transaction_menu == NULL) {

        selected_company_name = GetCurrentCompanyFromGraph();

        CreateNewDrawLayer();

        sell_transaction_menu = GetMenuWithChildsFromJsonLayer("SellTransactionMenu");
        AddMenuWithChildsToDrawLayer(sell_transaction_menu);
        InitializeTransactionMenuText();

    } else {

        ClearCurrentDrawLayer();
        sell_transaction_menu = NULL;

    }

}

void BuyMenu_BCB()
{

    if (buy_transaction_menu == NULL) {

        selected_company_name = GetCurrentCompanyFromGraph();

        CreateNewDrawLayer();

        buy_transaction_menu = GetMenuWithChildsFromJsonLayer("BuyTransactionMenu");
        AddMenuWithChildsToDrawLayer(buy_transaction_menu);
        InitializeTransactionMenuText();

    } else {

        ClearCurrentDrawLayer();
        buy_transaction_menu = NULL;

    }

}

void Sell_BCB()
{

    int amount_in_text_box = atoi(GetTextFromTextBox("SellTextBox"));
    AttemptToSubtractFromCurrentStock(GetCompanyNameViewing(), amount_in_text_box, CurrentStockPrice(selected_company_name));
    SellMenu_BCB();
    DisplaySellPopUp(amount_in_text_box, GetCompanyNameViewing()); 

}

void Buy_BCB()
{
    
    int amount_in_text_box = atoi(GetTextFromTextBox("BuyTextBox"));
    AttemptToAddFromCurrentStock(GetCompanyNameViewing(), amount_in_text_box, CurrentStockPrice(selected_company_name));
    BuyMenu_BCB();
    DisplayBuyPopUp(amount_in_text_box, GetCompanyNameViewing()); 

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

    stocks_menu           = NULL;
    sell_transaction_menu = NULL;
    buy_transaction_menu  = NULL;

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

}