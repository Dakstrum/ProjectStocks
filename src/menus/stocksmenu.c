#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"
#include "cache.h"
#include "graph.h"
#include "startup.h"
#include "dbaccount.h"
#include "account.h"
#include "simulation.h"

static MenuWithChilds *stocks_menu           = NULL;
static MenuWithChilds *sell_transaction_menu = NULL;
static MenuWithChilds *buy_transaction_menu  = NULL;

static float price_per_stock                 = 0.0;
static char *current_company_name            = NULL;
static DrawObject  *current_graph            = NULL;


static DrawObject *CompanyNameTextObject  = NULL;
static DrawObject *CompanyAboutTextObject = NULL;

void DisplayTempPopUp();
void DisplayGraph(char *company_name, TimeSpan time_span);
void DisplayCompanyScrollBox();
void AddCompanyContentToStocksScrollBox(DrawObject *object);
char *GetCurrentCompanyFromGraph();

void DisplayTempPopUp()
{

    DrawObject *popup_object = CreateNewDrawObject();

    popup_object->type   = POPUP;
    popup_object->x      = 0;
    popup_object->y      = 0;
    popup_object->width  = 1920;
    popup_object->height = 1080;
    
    popup_object->asset_path                 = "assets/images/generalpurposemenus/popups/genericpopup.png";
    popup_object->popup.diff_time_to_animate = 1000;
    popup_object->popup.diff_time_to_stay    = 2000;
    popup_object->popup.direction            = "Up";
    AddObjectToDrawLayer(popup_object);
}

void UpdateStocksStatsText(char *company_name)
{
    current_company_name = GetCurrentCompanyFromGraph();

    SetTextContent(CompanyNameTextObject, "%s", company_name);
    SetTextContent(CompanyAboutTextObject, "Dynamic Description of a company");
}

void InitializeStocksMenu() 
{ 
    
    if (CreateNewDrawLayer() == -1) {

        Log("STUB: StocksMenu could not create new draw layer");
        return;

    }

    stocks_menu = GetMenuWithChildsFromJsonLayer("StocksMenu");

    AddMenuWithChildsToDrawLayer(stocks_menu);
    DisplayCompanyScrollBox();
    DisplayGraph(GetStockNameFromStockId(1), ONE_DAY);

    CompanyNameTextObject  = GetDrawObjectFromJsonLayer("StocksMenuChangingCompanyNameText");
    CompanyAboutTextObject = GetDrawObjectFromJsonLayer("StocksMenuChangingAboutText");

    AddObjectToDrawLayer(CompanyNameTextObject);
    AddObjectToDrawLayer(CompanyAboutTextObject);

    UpdateStocksStatsText(GetStockNameFromStockId(1));

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

void LoadCompanyScrollBoxClick(char *scroll_box_content)
{

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

        current_company_name = GetCurrentCompanyFromGraph();
        price_per_stock      = CurrentStockPrice(current_company_name);

        CreateNewDrawLayer();
        sell_transaction_menu = GetMenuWithChildsFromJsonLayer("SellTransactionMenu");
        AddMenuWithChildsToDrawLayer(sell_transaction_menu);
        
    } else {

        ClearCurrentDrawLayer();
        sell_transaction_menu = NULL;

    }

}

void BuyMenu_BCB()
{

    if (buy_transaction_menu == NULL) {

        current_company_name   = GetCurrentCompanyFromGraph();
        price_per_stock        = CurrentStockPrice(current_company_name);

        CreateNewDrawLayer();
        buy_transaction_menu = GetMenuWithChildsFromJsonLayer("BuyTransactionMenu");
        AddMenuWithChildsToDrawLayer(buy_transaction_menu);
        
    } else {

        ClearCurrentDrawLayer();
        buy_transaction_menu = NULL;

    }

}

void Sell_BCB()
{

    int amount_in_text_box = atoi(GetTextFromTextBox("SellTextBox"));
    AttemptToSubtractFromCurrentStock(current_company_name, amount_in_text_box, price_per_stock);
    SellMenu_BCB();
    // TODO tell you what you sold or bought for how much
    DisplayTempPopUp(); 

}

void Buy_BCB()
{
    
    int amount_in_text_box = atoi(GetTextFromTextBox("BuyTextBox"));
    AttemptToAddFromCurrentStock(current_company_name, amount_in_text_box, price_per_stock);
    BuyMenu_BCB();
    // TODO tell you what you sold or bought for how much
    DisplayTempPopUp(); 

}

void OneD_BCB()
{
    current_company_name = GetCurrentCompanyFromGraph();
    RemoveDrawObject(current_graph);
    DisplayGraph(current_company_name, ONE_DAY);
    UpdateStocksStatsText(current_company_name);

}

void OneW_BCB()
{

    current_company_name = GetCurrentCompanyFromGraph();
    RemoveDrawObject(current_graph);
    DisplayGraph(current_company_name, ONE_WEEK);
    UpdateStocksStatsText(current_company_name);

}

void OneM_BCB()
{

    current_company_name = GetCurrentCompanyFromGraph();
    RemoveDrawObject(current_graph);
    DisplayGraph(current_company_name, ONE_MONTH);
    UpdateStocksStatsText(current_company_name);

}

void OneY_BCB()
{

    current_company_name = GetCurrentCompanyFromGraph();
    RemoveDrawObject(current_graph);
    DisplayGraph(current_company_name, ONE_YEAR);
    UpdateStocksStatsText(current_company_name);

}

void All_BCB()
{

    current_company_name = GetCurrentCompanyFromGraph();
    RemoveDrawObject(current_graph);
    DisplayGraph(current_company_name, ALL_TIME);
    UpdateStocksStatsText(current_company_name);

}

void CleanUpStocksMenu() 
{

    if (stocks_menu != NULL)
        free(stocks_menu);
    
    stocks_menu = NULL;
    
}