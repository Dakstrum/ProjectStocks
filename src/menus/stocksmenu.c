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

static MenuWithChilds *stocks_menu            = NULL;
static MenuWithChilds *sell_transaction_menu  = NULL;
static MenuWithChilds *buy_transaction_menu   = NULL;

static char *selected_company_name  = NULL;
static DrawObject *current_graph    = NULL;

static DrawObject *AccountMoneyTextObject = NULL;
static DrawObject *AccountDateTextObject  = NULL; 

static DrawObject *CompanyNameTextObject  = NULL;
static DrawObject *CompanyAboutTextObject = NULL;
static DrawObject *StockPriceTextObject   = NULL;
static DrawObject *StockChangeTextObject  = NULL;

static DrawObject *SelectedCompanyNameTextObject          = NULL;
static DrawObject *SelectedCompanyPerStockPriceTextObject = NULL;
static DrawObject *SelectedCompanyProjectedCostTextObject = NULL;


void DisplayTempPopUp();
void DisplayGraph(char *company_name, TimeSpan time_span);
void DisplayCompanyScrollBox();
void AddCompanyContentToStocksScrollBox(DrawObject *object);
char *GetCurrentCompanyFromGraph();
void StocksMenuRenderLogic();
void UpdateStocksStatsText(char *company_name);
void InitalizeStocksMenuText();
void ApplySelectedCompanyText();
void TransactionMenusRenderLogic();

void InitializeStocksMenu() 
{ 

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: StocksMenu could not create new draw layer");
        return;

    }

    stocks_menu = GetMenuWithChildsFromJsonLayer("StocksMenu");
    AddMenuWithChildsToDrawLayer(stocks_menu);
    
    DisplayCompanyScrollBox();
    DisplayGraph(GetCompanyNameViewing(), ONE_DAY);

    InitalizeStocksMenuText();
    UpdateStocksStatsText(GetCompanyNameViewing());

    InitializeDynamicObjects();

}

void StocksMenuRenderLogic()
{

    if (AccountMoneyTextObject == NULL)
        return;

    SetTextContent(StockChangeTextObject,  "%.2f", GetCurrentStockChange(GetCompanyNameViewing()));
    SetTextContent(AccountMoneyTextObject, "%.2f", GetAccountMoney());
    SetTextContent(AccountDateTextObject,  "%s",   GetDate());
    SetTextContent(StockPriceTextObject,   "%.2f", CurrentStockPrice(GetCompanyNameViewing()));

    TransactionMenusRenderLogic();

}

void TransactionMenusRenderLogic()
{
    
    if(buy_transaction_menu)
    {
        SetTextContent(SelectedCompanyPerStockPriceTextObject, "%.2f", CurrentStockPrice(selected_company_name));
        SetTextContent(SelectedCompanyProjectedCostTextObject, "%.2f", CurrentStockPrice(selected_company_name) * atoi(GetTextFromTextBox("BuyTextBox")));
    }
    if(sell_transaction_menu)
    {
        SetTextContent(SelectedCompanyPerStockPriceTextObject, "%.2f", CurrentStockPrice(selected_company_name));
        SetTextContent(SelectedCompanyProjectedCostTextObject, "%.2f", CurrentStockPrice(selected_company_name) * atoi(GetTextFromTextBox("SellTextBox")));

    }

}

void InitalizeStocksMenuText()
{

    CompanyNameTextObject  = GetObjectAndDraw("StocksMenuChangingCompanyNameText");
    CompanyAboutTextObject = GetObjectAndDraw("StocksMenuChangingAboutText");
    AccountMoneyTextObject = GetObjectAndDraw("StocksMenuAccountMoneyText");
    AccountDateTextObject  = GetObjectAndDraw("StocksMenuAccountDateText");
    StockPriceTextObject   = GetObjectAndDraw("StocksMenuCurrentStockPriceText");
    StockChangeTextObject  = GetObjectAndDraw("StocksMenuStockChangeText");

    SetTextContent(StockChangeTextObject, "%.2f", GetCurrentStockChange(GetCompanyNameViewing()));
    SetTextContent(AccountMoneyTextObject, "%.2f", GetAccountMoney());
    SetTextContent(StockPriceTextObject,   "%.2f", CurrentStockPrice(GetCompanyNameViewing()));

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

void UpdateStocksStatsText(char *company_name)
{

    SetTextContent(CompanyNameTextObject, "%s", company_name);
    SetTextContent(CompanyAboutTextObject, "Dynamic Description of a company");
    StocksMenuRenderLogic();

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
    UpdateStocksStatsText(scroll_box_content);

}

void DisplayCompanyScrollBox() 
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

    AddCompanyContentToStocksScrollBox(object);
    AddObjectToDrawLayer(object);

}

void AddCompanyContentToStocksScrollBox(DrawObject *object)
{

    for(int i = 0; i < GetAmountOfCompanies(); i++)
        object->scrollbox.text_content[i]  = GetStockNameFromStockId(i+1);

}

void SellMenu_BCB()
{

    if (sell_transaction_menu == NULL) {

        selected_company_name = GetCurrentCompanyFromGraph();

        CreateNewDrawLayer();

        sell_transaction_menu = GetMenuWithChildsFromJsonLayer("SellTransactionMenu");
        AddMenuWithChildsToDrawLayer(sell_transaction_menu);
        ApplySelectedCompanyText();

    } else {

        ClearCurrentDrawLayer();
        sell_transaction_menu = NULL;

    }

}

void ApplySelectedCompanyText()
{

    SelectedCompanyNameTextObject           = GetObjectAndDraw("SellTransactionMenuCompanyNameText");
    SelectedCompanyPerStockPriceTextObject  = GetObjectAndDraw("SellTransactionMenuPerShareText");
    SelectedCompanyProjectedCostTextObject  = GetObjectAndDraw("BuyTransactionMenuProjectedCostText");

    SetTextContent(SelectedCompanyNameTextObject  , "%s", selected_company_name);

}

void BuyMenu_BCB()
{

    if (buy_transaction_menu == NULL) {

        selected_company_name = GetCurrentCompanyFromGraph();

        CreateNewDrawLayer();

        buy_transaction_menu = GetMenuWithChildsFromJsonLayer("BuyTransactionMenu");
        AddMenuWithChildsToDrawLayer(buy_transaction_menu);
        ApplySelectedCompanyText();

    } else {

        ClearCurrentDrawLayer();
        buy_transaction_menu = NULL;

    }

}

void ChangeGraphTimespan(TimeSpan time_span)
{

    RemoveDrawObject(current_graph);
    DisplayGraph(GetCompanyNameViewing(), time_span);
    UpdateStocksStatsText(GetCompanyNameViewing());

}

void Sell_BCB()
{

    int amount_in_text_box = atoi(GetTextFromTextBox("SellTextBox"));
    AttemptToSubtractFromCurrentStock(GetCompanyNameViewing(), amount_in_text_box, CurrentStockPrice(selected_company_name));
    SellMenu_BCB();
    // TODO tell you what you sold or bought for how much
    DisplaySellPopUp(amount_in_text_box, GetCompanyNameViewing()); 

}

void Buy_BCB()
{
    
    int amount_in_text_box = atoi(GetTextFromTextBox("BuyTextBox"));
    AttemptToAddFromCurrentStock(GetCompanyNameViewing(), amount_in_text_box, CurrentStockPrice(selected_company_name));
    BuyMenu_BCB();
    // TODO tell you what you sold or bought for how much
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

    if (stocks_menu != NULL)
        free(stocks_menu);
    
    stocks_menu = NULL;
    
}