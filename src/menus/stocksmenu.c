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

static MenuWithChilds *stocks_menu           = NULL;
static MenuWithChilds *sell_menu = NULL;
static MenuWithChilds *buy_menu  = NULL;

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

    stocks_menu = GetJSONMenuAndAddToDrawLayer("StocksMenu");
    InitalizeStocksMenuText();
    InitializeCompanyScrollBox();

    DisplayGraph(GetCompanyNameViewing(), ONE_DAY);
    PopulateStockStatsText(GetCompanyNameViewing());

    InitializeSpeedSelectObject("StocksMenu");

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
    company_about_textobject = GetJSONObjectAndAddToDrawLayer("StocksMenuAboutCompanyTextObject");
    player_money_textobject  = GetJSONObjectAndAddToDrawLayer("StocksMenuAccountMoneyTextObject");
    player_date_textobject   = GetJSONObjectAndAddToDrawLayer("StocksMenuAccountDateTextObject");
    stock_price_textobject   = GetJSONObjectAndAddToDrawLayer("StocksMenuStockPriceTextObject");
    stock_change_textobject  = GetJSONObjectAndAddToDrawLayer("StocksMenuPriceChangeTextObject");

    SetTextContent(stock_change_textobject, "%.2f", GetCurrentStockChange(GetCompanyNameViewing()));
    SetTextContent(player_money_textobject, "%.2f", GetAccountMoney());
    SetTextContent(stock_price_textobject,  "%.2f", CurrentStockPrice(GetCompanyNameViewing()));

}

void InitializeBuyMenuText()
{
    InitializeTransactionMenuTextBoxes();

    transaction_menu_company_name_textobject = GetJSONObjectAndAddToDrawLayer("BuyMenuCompanyNameTextObject");
    transaction_menu_pershare_textobject     = GetJSONObjectAndAddToDrawLayer("BuyMenuPricePerTextObject");
    transaction_menu_projected_textobject    = GetJSONObjectAndAddToDrawLayer("BuyMenuPriceAllTextObject");

    SetTextContent(transaction_menu_company_name_textobject, "%s", GetCompanyAbbreviation(GetCompanyId(selected_company_name)));

}

void InitializeSellMenuText()
{
    InitializeTransactionMenuTextBoxes();

    transaction_menu_company_name_textobject = GetJSONObjectAndAddToDrawLayer("SellMenuCompanyNameTextObject");
    transaction_menu_pershare_textobject     = GetJSONObjectAndAddToDrawLayer("SellMenuPricePerTextObject");
    transaction_menu_projected_textobject    = GetJSONObjectAndAddToDrawLayer("SellMenuPriceAllTextObject");
    transaction_menu_amountowned_textobject  = GetJSONObjectAndAddToDrawLayer("SellMenuAmountOwnedTextObject");

    SetTextContent(transaction_menu_company_name_textobject, "%s", GetCompanyAbbreviation(GetCompanyId(selected_company_name)));
    SetTextContent(transaction_menu_amountowned_textobject, "%d", GetOwnedStockAmount(selected_company_name));

}



void DisplayPopup(char str[50]) 
{
    DrawObject *object = CreateNewPopup();
    SetPopupText(object, str);
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

    char *company_name = GetCompanyName(index+1);
    SetCompanyIdViewing(GetCompanyId(company_name));

    RemoveDrawObject(current_graph);
    DisplayGraph(company_name, ONE_DAY);
    PopulateStockStatsText(company_name);

}

void InitializeCompanyScrollBox()
{

    DrawObject *object = CreateScrollBoxObject();

    object->x          = 2;
    object->y          = 230;
    object->width      = 288;
    object->height     = 603;
    object->asset_path = "assets/images/companyicons/StocksBox.png";

    object->scrollbox.num_items    = GetNumCompanies();
    object->scrollbox.box_click    = &LoadCompanyScrollBoxClick;
    object->scrollbox.text_content = malloc(sizeof(char *) * GetNumCompanies());

    PopulateStocksScrollBox(object);
    AddObjectToDrawLayer(object);

}

void PopulateStocksScrollBox(DrawObject *object)
{
    for(int i = 0; i < GetNumCompanies(); i++)
        object->scrollbox.text_content[i] = GetCompanyAbbreviation(i+1);

}

void ChangeGraphTimespan(TimeSpan time_span)
{

    RemoveDrawObject(current_graph);
    DisplayGraph(GetCompanyNameViewing(), time_span);
    PopulateStockStatsText(GetCompanyNameViewing());

}


void InitializeTransactionMenuTextBoxes()
{
    if(buy_menu)
    {

        DrawObject *buy_tb = CreateTextBoxObject("BuyTextBox", "", 10, TEXTBOX_ACCEPT_ALPHABET_CHARACTERS | TEXTBOX_ACCEPT_NUMBER_CHARACTERS);
        buy_tb->x          = 955;
        buy_tb->y          = 395;
        buy_tb->width      = 145;
        buy_tb->height     = 22;

        AddObjectToDrawLayer(buy_tb);

    }

    if(sell_menu)
    {

        DrawObject *sell_tb = CreateTextBoxObject("SellTextBox", "", 10, TEXTBOX_ACCEPT_ALPHABET_CHARACTERS | TEXTBOX_ACCEPT_NUMBER_CHARACTERS);
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
    if (amount_in_text_box < 0)
        return;

    char str[50];
    float current_stock_price = CurrentStockPrice(selected_company_name);
    bool successful = AttemptToSubtractFromCurrentStock(GetCompanyNameViewing(), amount_in_text_box, CurrentStockPrice(selected_company_name));

    if (successful) {

        SellMenu_BCB();
        sprintf(str, "Sold %d of %s", amount_in_text_box, GetCompanyNameViewing());
        DisplayPopup(str);
        AddMoney(amount_in_text_box * current_stock_price);

    } else {

        sprintf(str, "Unable to sell stocks");
        DisplayPopup(str);

    }


}

void Buy_BCB()
{
    
    int amount_in_text_box    = atoi(GetTextFromTextBox("BuyTextBox"));
    float current_stock_price = CurrentStockPrice(selected_company_name);

    char str[50];
    if (CanMakeTransaction(amount_in_text_box * current_stock_price)) {

        AttemptToAddFromCurrentStock(GetCompanyNameViewing(), amount_in_text_box, current_stock_price);
        BuyMenu_BCB();

        sprintf(str, "Bought %d of %s", amount_in_text_box, GetCompanyNameViewing());
        DisplayPopup(str);

        SubtractMoney(amount_in_text_box * current_stock_price);

    } else {

        sprintf(str, "Unable to purchase stock");
        DisplayPopup(str);

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

    stocks_menu = NULL;
    sell_menu   = NULL;
    buy_menu    = NULL;

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

}