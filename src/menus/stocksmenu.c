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
#include "dbaccess.h"
#include "account.h"
#include "simulation.h"

static MenuWithChilds *stocks_menu           = NULL;
static MenuWithChilds *sell_transaction_menu = NULL;
static MenuWithChilds *buy_transaction_menu  = NULL;

static char *current_company_name;
static float price_per_stock;
DrawObject *graph;

void DisplayTempPopUp()
{

    DrawObject *popup_object = CreateNewDrawObject();

    popup_object->type                 = POPUP;
    popup_object->should_this_be_drawn = true;
    popup_object->x                    = 0;
    popup_object->y                    = 0;

    popup_object->width  = 1920;
    popup_object->height = 1080;
    
    popup_object->asset_path                 = "assets/images/generalpurposemenus/popups/genericpopup.png";
    popup_object->popup.diff_time_to_animate = 1000;
    popup_object->popup.diff_time_to_stay    = 2000;
    popup_object->popup.direction            = "Up";
    AddObjectToDrawLayer(popup_object);

}

void DisplayGraph(char *company_name, int x, int y)
{

    graph = GetGraphDrawObject(company_name, ONE_DAY, 961, 373);
    if (graph != NULL) {

        graph->x = 415;
        graph->y = 234;
        AddObjectToDrawLayer(graph);

    }

}

void LoadCompanyScrollBoxClick(char *scroll_box_content)
{
    RemoveDrawObject(graph);
    DisplayGraph(scroll_box_content ,414, 234);

}

void AddCompanyContentToStocksScrollBox(DrawObject *object)
{

    for(int i; i < GetAmountOfCompanies(); i++)
        object->scrollbox.text_content[i]  = GetStockNameFromStockId(i+1);

}

void DisplayCompanyScrollBox() 
{

    DrawObject *object = CreateScrollBoxObject();

    object->x          = 2;
    object->y          = 230;
    object->width      = 288;
    object->height     = 603;
    object->asset_path = "assets/images/companyicons/StocksBox.png";

    object->scrollbox.num_items        = GetAmountOfCompanies();
    object->scrollbox.box_click        = &LoadCompanyScrollBoxClick;
    object->scrollbox.text_content     = malloc(sizeof(char *) * 2);

    AddCompanyContentToStocksScrollBox(object);
    AddObjectToDrawLayer(object);

}


char *GetCurrentCompanyFromGraph()
{

    DrawObjectTypeCollection *current_draw_layer_graphs = GetObjectsByType(GRAPH);
    return current_draw_layer_graphs->objects[0]->graph.company;

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
    DisplayGraph(GetStockNameFromStockId(1) ,414, 234);
    DisplayTempPopUp();

}


float GetPricePerStock(char *company)
{

    return CurrentStockPrice(company);

}


void StocksSellButtonCallBack()
{

    if (sell_transaction_menu == NULL) {

        current_company_name = GetCurrentCompanyFromGraph();
        price_per_stock      = GetPricePerStock(current_company_name);

        CreateNewDrawLayer();
        sell_transaction_menu = GetMenuWithChildsFromJsonLayer("SellTransactionMenu");
        AddMenuWithChildsToDrawLayer(sell_transaction_menu);
        
    } else {

        ClearCurrentDrawLayer();
        sell_transaction_menu = NULL;

    }

}

void StocksBuyButtonCallBack()
{

    if (buy_transaction_menu == NULL) {

        current_company_name   = GetCurrentCompanyFromGraph();
        price_per_stock        = GetPricePerStock(current_company_name);

        CreateNewDrawLayer();
        buy_transaction_menu = GetMenuWithChildsFromJsonLayer("BuyTransactionMenu");
        AddMenuWithChildsToDrawLayer(buy_transaction_menu);
        
    } else {

        ClearCurrentDrawLayer();
        buy_transaction_menu = NULL;

    }

}

void MakeSellTransactionButtonCallBack()
{

    int amount_in_text_box = atoi(GetTextFromTextBox("SellTextBox"));
    AttemptToSubtractFromCurrentStock(current_company_name, amount_in_text_box, price_per_stock);
    StocksSellButtonCallBack();

}

void MakeBuyTransactionButtonCallBack()
{
    
    int amount_in_text_box = atoi(GetTextFromTextBox("BuyTextBox"));
    AttemptToAddFromCurrentStock(current_company_name, amount_in_text_box, price_per_stock);
    StocksBuyButtonCallBack();

}

void CleanUpStocksMenu() 
{

    if (stocks_menu != NULL)
        free(stocks_menu);
    
    stocks_menu = NULL;
    
}