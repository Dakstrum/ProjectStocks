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

char *company;
int price_per_stock;
int amount_in_text_box;

void DisplayTempPopUp();
void GetCurrentGraphAndCompanyInfo();

void InitializeStocksMenu() 
{ 
    
    if (CreateNewDrawLayer() == -1) {

        Log("STUB: StocksMenu could not create new draw layer");
        return;

    }

    stocks_menu = GetMenuWithChildsFromJsonLayer("StocksMenu");
    AddMenuWithChildsToDrawLayer(stocks_menu);
    DisplayTempPopUp();

    DrawObject *graph = GetGraphDrawObject("WeBeHard", ONE_DAY, 961, 373);
    if (graph != NULL) {

        graph->x = 415;
        graph->y = 234;
        AddObjectToDrawLayer(graph);

    }

}



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

void StocksSellButtonCallBack()
{

    if (sell_transaction_menu == NULL) {

        CreateNewDrawLayer();
        sell_transaction_menu = GetMenuWithChildsFromJsonLayer("SellTransactionMenu");
        AddMenuWithChildsToDrawLayer(sell_transaction_menu);
        
    } else {

        ClearCurrentDrawLayer();
        sell_transaction_menu = NULL;

    }

}

DrawObjectTypeCollection *current_draw_layer_graphs;

void StocksBuyButtonCallBack()
{
    
    GetCurrentGraphAndCompanyInfo();
    
    if (buy_transaction_menu == NULL) {

        CreateNewDrawLayer();
        buy_transaction_menu = GetMenuWithChildsFromJsonLayer("BuyTransactionMenu");
        AddMenuWithChildsToDrawLayer(buy_transaction_menu);
        
    } else {

        ClearCurrentDrawLayer();
        buy_transaction_menu = NULL;

    }

}

void GetCurrentGraphAndCompanyInfo()
{

    if(DoesObjectExistInCurrentDrawLayer("BuyTransactionMenu")) {

        Log("You are clicking out of Menu");

    } else {

        current_draw_layer_graphs = GetObjectsByType(GRAPH);

        company = current_draw_layer_graphs->objects[0]->graph.company;
        price_per_stock  = CurrentStockPrice(company);

        LogF("Company = %s | Price = %d", company, price_per_stock);

        

    }

}

void MakeSellTransactionButtonCallBack()
{

    amount_in_text_box = atoi(GetTextFromTextBox("SellTextBox"));

    //SubtractOwnedStock(amount_in_text_box);
    AttemptToSubtractFrom(amount_in_text_box, price_per_stock);
    //InsertStockTransaction(1,1,9, amount_in_text_box, 1, GetGameTime());

    StocksSellButtonCallBack();

}

void MakeBuyTransactionButtonCallBack()
{

    amount_in_text_box = atoi(GetTextFromTextBox("BuyTextBox"));

    //AddOwnedStock(1,1, amount_in_text_box);
    AttemptToAddFrom(amount_in_text_box, price_per_stock);
    //InsertStockTransaction(1,1, amount_in_text_box * price_per_stock, amount_in_text_box, GetGameTime());

    StocksBuyButtonCallBack();

}

void CleanUpStocksMenu() 
{

    if (stocks_menu != NULL)
        free(stocks_menu);
    
    stocks_menu = NULL;
    
}

