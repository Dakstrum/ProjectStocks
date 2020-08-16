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

void TempCreateWeatherBitMaps(); //TEMP

void InitializeNewsMenu() 
{ 

    if (CreateNewDrawLayer() == -1) {

        Log("STUB: StocksMenu could not create new draw layer");
        return;

    }
    
    news_menu = GetJSONMenuAndAddToDrawLayer("NewsMenu");

    InitalizeNewsMenuText();
    InitializeSpeedSelectObject("NewsMenu");

    NewsMenuRenderLogic();
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



void TempCreateWeatherBitMaps() //This function is nasty. Will change when I can create bitmaps and not menus
{

    

    sun_bitmap = CreateNewDrawObject();
    sun_bitmap->type                            = MENU;
    sun_bitmap->x                               = 1292;
    sun_bitmap->y                               = 417;
    sun_bitmap->width                           = 30;
    sun_bitmap->height                          = 30;
    
    mon_bitmap = CreateNewDrawObject();
    mon_bitmap->type                            = MENU;
    mon_bitmap->x                               = 1374;
    mon_bitmap->y                               = 417;
    mon_bitmap->width                           = 30;
    mon_bitmap->height                          = 30;
    
    tues_bitmap = CreateNewDrawObject();
    tues_bitmap->type                            = MENU;
    tues_bitmap->x                               = 1470;
    tues_bitmap->y                               = 417;
    tues_bitmap->width                           = 30;
    tues_bitmap->height                          = 30;
    
    wed_bitmap = CreateNewDrawObject();
    wed_bitmap->type                            = MENU;
    wed_bitmap->x                               = 1561;
    wed_bitmap->y                               = 417;
    wed_bitmap->width                           = 30;
    wed_bitmap->height                          = 30;

    thurs_bitmap = CreateNewDrawObject();
    thurs_bitmap->type                          = MENU;
    thurs_bitmap->x                             = 1660;
    thurs_bitmap->y                             = 417;
    thurs_bitmap->width                         = 30;
    thurs_bitmap->height                        = 30;

    fri_bitmap = CreateNewDrawObject();
    fri_bitmap->type                          = MENU;
    fri_bitmap->x                             = 1740;
    fri_bitmap->y                             = 417;
    fri_bitmap->width                         = 30;
    fri_bitmap->height                        = 30;

    sat_bitmap = CreateNewDrawObject();
    sat_bitmap->type                          = MENU;
    sat_bitmap->x                             = 1813;
    sat_bitmap->y                             = 417;
    sat_bitmap->width                         = 30;
    sat_bitmap->height                        = 30;
    
    srand(time(NULL));

    DrawObject *all_weather_bitmaps[7] = {sun_bitmap, mon_bitmap, tues_bitmap, wed_bitmap, thurs_bitmap, fri_bitmap, sat_bitmap};
    DrawObject *all_weather_temps_textobjects[7]   = {sun_temp_textobject, mon_temp_textobject , tues_temp_textobject, wed_temp_textobject, thurs_temp_textobject, fri_temp_textobject, sat_temp_textobject};

    for(int i = 0; i < 7; i++)
    {

        int weather_num;
        weather_num = rand() % 3 + 0;

        if(weather_num == 0)
        {
            all_weather_bitmaps[i]->asset_path = "assets/images/newsmenu/newsmenuassets/sunny.png";
            SetTextContent(all_weather_temps_textobjects[i], "%s", "90°");
        }


        if(weather_num == 1)
        {
            all_weather_bitmaps[i]->asset_path = "assets/images/newsmenu/newsmenuassets/cloudy.png";
            SetTextContent(all_weather_temps_textobjects[i], "%s", "70°");
        }

        if(weather_num == 2)
        {
            all_weather_bitmaps[i]->asset_path = "assets/images/newsmenu/newsmenuassets/rain.png";
            SetTextContent(all_weather_temps_textobjects[i], "%s", "60°");
        }

        AddObjectToDrawLayer(all_weather_bitmaps[i]);
    }

}

void CleanNewsMenu()
{

    player_money_textobject = NULL;
    player_date_textobject  = NULL;

}