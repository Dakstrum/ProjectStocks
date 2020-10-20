#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"
#include "text.h"
#include "account.h"
#include "generalpurposemenus.h"
#include "drawlayerutils.h"
#include "menu.h"
#include "textbox.h"
#include "dbevents.h"
#include "dbevents.h"
#include <time.h>

static MenuWithChilds *news_menu = NULL;

static DrawObject *player_money_textobject = NULL;
static DrawObject *player_date_textobject  = NULL;

static DrawObject *sun_temp_textobject    = NULL;
static DrawObject *mon_temp_textobject    = NULL;
static DrawObject *tues_temp_textobject   = NULL;
static DrawObject *wed_temp_textobject    = NULL;
static DrawObject *thurs_temp_textobject  = NULL;
static DrawObject *fri_temp_textobject    = NULL;
static DrawObject *sat_temp_textobject    = NULL;

static DrawObject *sun_bitmap    = NULL;
static DrawObject *mon_bitmap    = NULL;
static DrawObject *tues_bitmap   = NULL;
static DrawObject *wed_bitmap    = NULL;
static DrawObject *thurs_bitmap  = NULL;
static DrawObject *fri_bitmap    = NULL;
static DrawObject *sat_bitmap    = NULL;

void InitalizeNewsMenuText();
void NewsMenuRenderLogic();
void InitializeSearchTextBox();

void TempCreateWeatherBitMaps();
void InitalizeNewsMenuCategoryScrollbox();

void InitializeNewsMenu() 
{ 

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: StocksMenu could not create new draw layer");
        return;

    }
    
    news_menu = GetJSONMenuAndAddToDrawLayer("NewsMenu");
    InitalizeNewsMenuCategoryScrollbox();
    InitalizeNewsMenuText();
    InitializeSpeedSelectObject("NewsMenu");

    NewsMenuRenderLogic();

    InitializeSearchTextBox();
}

void InitalizeNewsMenuText()
{

    player_money_textobject = GetJSONObjectAndAddToDrawLayer("NewsMenuAccountMoneyTextObject");
    player_date_textobject  = GetJSONObjectAndAddToDrawLayer("NewsMenuAccountDateTextObject");

    sun_temp_textobject     = GetJSONObjectAndAddToDrawLayer("NewsMenusuntTextObject");
    mon_temp_textobject     = GetJSONObjectAndAddToDrawLayer("NewsMenumontTextObject");
    tues_temp_textobject    = GetJSONObjectAndAddToDrawLayer("NewsMenutuestTextObject");
    wed_temp_textobject     = GetJSONObjectAndAddToDrawLayer("NewsMenuwedtTextObject");
    thurs_temp_textobject   = GetJSONObjectAndAddToDrawLayer("NewsMenuthurstTextObject");
    fri_temp_textobject     = GetJSONObjectAndAddToDrawLayer("NewsMenufritTextObject");
    sat_temp_textobject     = GetJSONObjectAndAddToDrawLayer("NewsMenusattTextObject");

    TempCreateWeatherBitMaps();

}

void NewsMenuRenderLogic()
{

    if (player_money_textobject == NULL)
        return;
    
    SetTextContent(player_money_textobject, "%.2f", GetAccountMoney());
    SetTextContent(player_date_textobject,  "%s",   GetDate());

}

void NewsMenuCompanyScrollBoxClick(char *scroll_box_content, unsigned short int index)
{

    //char *company_name = GetCompanyName(index+1);
    //SetCompanyIdViewing(GetCompanyId(company_name));
    //ClearAccountHistoryDisplay();
    //PopulateSelectedStockHistoryDisplay(company_name);

    //SetTextContent(company_name_textobject, "%s", company_name);
    //SetTextContent(company_about_textobject, "%s", GetCompanyDescriptionRef(GetCompanyId(GetCompanyNameViewing())));

}

void InitalizeNewsMenuCategoryScrollbox() 
{

    AddObjectToDrawLayer(GetCategoryScrollbox(2, 230, &NewsMenuCompanyScrollBoxClick));

}


void TempCreateWeatherBitMaps() //This function is nasty. Will change when I can create bitmaps and not menus
{

    sun_bitmap   = CreateNewDrawObject();    
    mon_bitmap   = CreateNewDrawObject();    
    tues_bitmap  = CreateNewDrawObject();    
    wed_bitmap   = CreateNewDrawObject();
    thurs_bitmap = CreateNewDrawObject();
    fri_bitmap   = CreateNewDrawObject();
    sat_bitmap   = CreateNewDrawObject();  

    srand(time(NULL));

    int week_bitmap_x[7] = {1292, 1374, 1470, 1561, 1660, 1740, 1813};

    DrawObject *all_weather_bitmaps[7] = {sun_bitmap, mon_bitmap, tues_bitmap, wed_bitmap, thurs_bitmap, fri_bitmap, sat_bitmap};
    DrawObject *all_weather_temps_textobjects[7]   = {sun_temp_textobject, mon_temp_textobject , tues_temp_textobject, wed_temp_textobject, thurs_temp_textobject, fri_temp_textobject, sat_temp_textobject};

    for(int i = 0; i < 7; i++)
    {

        all_weather_bitmaps[i]->type   = MENU;
        all_weather_bitmaps[i]->x      = week_bitmap_x[i];
        all_weather_bitmaps[i]->y      = 417;
        all_weather_bitmaps[i]->width  = 30;
        all_weather_bitmaps[i]->height = 30;

        int weather_num;
        weather_num = rand() % 3 + 0;

        if(weather_num == 0) {

            all_weather_bitmaps[i]->asset_path = "assets/images/newsmenu/newsmenuassets/sunny.png";
            SetTextContent(all_weather_temps_textobjects[i], "%d", rand() % (99 + 1 - 75) + 75);
            
        }

        
        if(weather_num == 1) {

            all_weather_bitmaps[i]->asset_path = "assets/images/newsmenu/newsmenuassets/cloudy.png";
            SetTextContent(all_weather_temps_textobjects[i], "%d", rand() % (75 + 1 - 70) + 70);
            
        }

        if(weather_num == 2) {

            all_weather_bitmaps[i]->asset_path = "assets/images/newsmenu/newsmenuassets/rain.png";
            SetTextContent(all_weather_temps_textobjects[i], "%d", rand() % (70 + 1 - 60) + 60);

        }
        
        AddObjectToDrawLayer(all_weather_bitmaps[i]);

    }

}

void InitializeSearchTextBox()
{

    DrawObject *savename_tb = CreateTextBoxObject("SearchTextBox", "", 38, TEXTBOX_ACCEPT_ALPHABET_CHARACTERS | TEXTBOX_ACCEPT_NUMBER_CHARACTERS);
    savename_tb->x          = 561;
    savename_tb->y          = 171;
    savename_tb->width      = 440;
    savename_tb->height     = 30;

    AddObjectToDrawLayer(savename_tb);

}

void CleanNewsMenu()
{

    player_money_textobject = NULL;
    player_date_textobject  = NULL;

    sun_temp_textobject    = NULL;
    mon_temp_textobject    = NULL;
    tues_temp_textobject   = NULL;
    wed_temp_textobject    = NULL;
    thurs_temp_textobject  = NULL;
    fri_temp_textobject    = NULL;
    sat_temp_textobject    = NULL;

    sun_bitmap    = NULL;
    mon_bitmap    = NULL;
    tues_bitmap   = NULL;
    wed_bitmap    = NULL;
    thurs_bitmap  = NULL;
    fri_bitmap    = NULL;
    sat_bitmap    = NULL;

    news_menu = NULL;

}
