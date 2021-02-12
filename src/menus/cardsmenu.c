#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"
#include "cache.h"
#include "graph.h"
#include "startup.h"
#include "dbcompany.h"
#include "dbaccount.h"
#include "account.h"
#include "game.h"
#include "simulation.h"
#include "scrollbox.h"
#include "text.h"
#include "generalpurposemenus.h"
#include "drawlayerutils.h"
#include "dbcard.h"
#include "popup.h"
#include "button.h"

static MenuWithChilds *cards_menu      = NULL;
static MenuWithChilds *apply_card_menu = NULL;

static DrawObject *player_money_textobject = NULL;
static DrawObject *player_date_textobject  = NULL; 

static DrawObject *card_title_textobject   = NULL;
static DrawObject *card_desc_textobject    = NULL;

static DrawObject *card_bitmap    = NULL;

static int current_button_idx = -1;

void CardsMenu_RenderLogic();

void CardsMenu_InitPositiveScrollbox();
void CardsMenu_InitNegativeScrollBox();

void CardsMenu_InitCompanyScrollbox();

void CardsMenu_InitTextAndButtons();
void CardsMenu_InitCardBitmapAndText(char* card_title);
void CardsMenu_PickCompanyMenu_CB();
void CardsMenu_Clean();

void CardsMenu_Init() 
{

    CardsMenu_Clean();
    if (CreateNewDrawLayer() == -1) {

        Log("STUB: cards Menu could not create new draw layer");
        return;
    }

    cards_menu = GetJSONMenuAndAddToDrawLayer("CardsMenu");
    //CardsMenu_InitPositiveScrollbox();
    //CardsMenu_InitNegativeScrollBox();
    //CardsMenu_InitTextAndButtons(); 

    GeneralPurposeMenus_InitSpeedSelectObject("CardsMenu");

    //dbcard_apply_card(1,1);

}

void CardsMenu_RenderLogic()
{

    if (player_money_textobject == NULL)
        return;
    
    SetTextContent(player_money_textobject, "%.2f", Account_GetMoney(Account_GetPlayerId()));
    SetTextContent(player_date_textobject,  "%s",   Game_GetDate());

}

void CardsMenu_InitTextAndButtons()
{

    player_money_textobject = GetJSONObjectAndAddToDrawLayer("CardsMenuAccountMoneyTextObject");
    player_date_textobject  = GetJSONObjectAndAddToDrawLayer("CardsMenuAccountDateTextObject");
    card_title_textobject   = GetJSONObjectAndAddToDrawLayer("CardsMenuCardTitleTextObject");

    card_desc_textobject    = GetDrawObjectFromJsonLayer("CardsMenuCardDescriptionTextObject");
    card_desc_textobject->width = 400;
    AddObjectToDrawLayer(card_desc_textobject);

}

void CardsMenu_PopulateCompanyScrollbox(DrawObject *object)
{

    for(int i = 0; i < GetNumCompanies(); i++)
        object->scrollbox.text_content[i]  = GetCompanyName(i+1);

}

void CardsMenu_CompanyScrollboxClick(char *scroll_box_content, unsigned short int index)
{

    uint32_t company_id = GetCompanyId(scroll_box_content);
    dbcard_apply_card(Account_GetPlayerId(), GetPlayerCardId(GetCardId(card_title_textobject->text.content)), company_id);
    CardsMenu_PickCompanyMenu_CB();
    DisplayPopupOnDrawLayer("Added Card to Company", "assets/images/generalpurposemenus/popups/greenpopup.png");

}

void CardsMenu_InitCompanyScrollbox() 
{

    DrawObject *object = Scrollbox_Create();

    object->x          = 757;
    object->y          = 363;
    object->width      = 288;
    object->height     = 350;
    object->asset_path = "assets/images/stocksmenu/stocksmenuassets/StocksBox.png";

    object->scrollbox.num_items    = GetNumCompanies();
    object->scrollbox.box_click    = &CardsMenu_CompanyScrollboxClick;
    object->scrollbox.text_content = malloc(sizeof(char *) * GetNumCompanies());

    CardsMenu_PopulateCompanyScrollbox(object);
    AddObjectToDrawLayer(object);

}

void CardsMenu_LoadCard(char *scroll_box_content, unsigned short int index)
{

    if (card_title_textobject == NULL || card_desc_textobject == NULL)
        return;

    current_button_idx = index;

    CardsMenu_InitCardBitmapAndText(scroll_box_content);

}

void CardsMenu_PopulatePositiveCardScrollbox(DrawObject *object)
{

    PlayerCard *temp  = GetAllPlayerCards();
    int scrollbox_num = 0;

    for(int i = 0; i < GetNumOfPlayerCards(); i++) {

        if(GetCardType(temp[i].card_id) == 1) {

            object->scrollbox.text_content[scrollbox_num] = GetFormattedPointer(GetCardTitle(temp[i].card_id));
            scrollbox_num++;

        }

    }

}

void CardsMenu_InitPositiveScrollbox() 
{

    DrawObject *object = Scrollbox_Create();

    object->x          = 50;
    object->y          = 233;
    object->width      = 288;
    object->height     = 809;
    object->asset_path = "assets/images/stocksmenu/stocksmenuassets/StocksBox.png";

    object->scrollbox.num_items        = GetNumOfPlayerPositiveCards();
    object->scrollbox.box_click        = &CardsMenu_LoadCard;
    object->scrollbox.text_content     = malloc(sizeof(char *) * GetNumOfPlayerPositiveCards());
    
    CardsMenu_PopulatePositiveCardScrollbox(object);
    AddObjectToDrawLayer(object);

}

void CardsMenu_PopulateNegativeScrollbox(DrawObject *object)
{
    
    PlayerCard *temp  = GetAllPlayerCards();
    int cards_num     = GetNumOfPlayerCards();
    int scrollbox_num = 0;


    for(int i = 0; i < cards_num; i++)  {

        if(GetCardType(temp[i].card_id) == 0) {

            object->scrollbox.text_content[scrollbox_num] = GetFormattedPointer(GetCardTitle(temp[i].card_id));
            scrollbox_num++;

        }

    }

}

void CardsMenu_InitNegativeScrollBox() 
{

    DrawObject *object = Scrollbox_Create();

    object->x          = 480;
    object->y          = 233;
    object->width      = 288;
    object->height     = 809;
    object->asset_path = "assets/images/stocksmenu/stocksmenuassets/StocksBox.png";

    object->scrollbox.num_items        = GetNumOfPlayerNegativeCards();
    object->scrollbox.box_click        = &CardsMenu_LoadCard;
    object->scrollbox.text_content     = malloc(sizeof(char *) * GetNumOfPlayerNegativeCards());

    CardsMenu_PopulateNegativeScrollbox(object);
    AddObjectToDrawLayer(object);

}

void CardsMenu_PickCompanyMenu_CB()
{
    if (current_button_idx == -1) {

        DisplayPopupOnDrawLayer("No Card Selected", "assets/images/generalpurposemenus/popups/redpopup.png");
        return;

    }
   
    if (apply_card_menu == NULL) {

        CreateNewDrawLayer();
        apply_card_menu = GetMenuWithChildsFromJsonLayer("ApplyCardMenu");
        AddMenuWithChildsToDrawLayer(apply_card_menu);
        CardsMenu_InitCompanyScrollbox(); 
        
    } else {

        ClearCurrentDrawLayer();
        apply_card_menu = NULL;
        ClearCurrentDrawLayer();
        cards_menu      = NULL;
        CardsMenu_Init();
        current_button_idx = -1;
        
    }

}

void CardsMenu_InitCardBitmapAndText(char* card_title)
{
    

    if(card_bitmap) {
        
        RemoveDrawObject(card_bitmap);
        card_bitmap = NULL;
    }
    
    card_bitmap = CreateNewDrawObject();
    card_bitmap->type                            = MENU;
    card_bitmap->x                               = 1458;
    card_bitmap->y                               = 218;
    card_bitmap->width                           = 352;
    card_bitmap->height                          = 496;
    card_bitmap->asset_path = GetCardPath(GetCardId(card_title));

    AddObjectToDrawLayer(card_bitmap);
    
    SetTextContent(card_title_textobject, "%s", card_title);
    SetTextContent(card_desc_textobject,  "%s", GetCardDescription(GetCardId(card_title)));

}

void CardsMenu_Clean()
{

    player_money_textobject  = NULL;
    player_date_textobject   = NULL; 
    card_title_textobject    = NULL;
    card_desc_textobject     = NULL;

    card_bitmap    = NULL;

    cards_menu      = NULL;
    apply_card_menu = NULL;

    current_button_idx = -1;

}
