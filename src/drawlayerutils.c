#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "jsonlayer.h"
#include "drawlayers.h"
#include "log.h"
#include "dbaccount.h"
#include "text.h"
#include "scrollbox.h"
#include "account.h"
#include "simulation.h"
#include "scrollbox.h"
#include "drawobject.h"
#include "dbcompany.h"
#include "vector.h"
#include "generalpurposemenus.h"

DrawObject *GetJSONObjectAndAddToDrawLayer(char* object_name)
{

    DrawObject *object = GetDrawObjectFromJsonLayer(object_name);
    AddObjectToDrawLayer(object);
    return object;

}

MenuWithChilds *GetJSONMenuAndAddToDrawLayer(char* menu_name)
{

    MenuWithChilds *menu_object = GetMenuWithChildsFromJsonLayer(menu_name);
    AddMenuWithChildsToDrawLayer(menu_object);
    return menu_object;

}

void InitScrollboxVectors(DrawObject *object)
{

	for (size_t i = 0; i < object->scrollbox.num_items;i++) {

		object->scrollbox.text_content[i] = Vector_Create(sizeof(ScrollboxText) * 4);
		object->scrollbox.icons[i]        = Vector_Create(sizeof(ScrollboxIcon) * 4);

	}

}

void SetScrollboxTextContentForCompanies(DrawObject *object) 
{

    int num_companies = GetNumCompanies();
	for (size_t i = 0; i < num_companies;i++) {

		Vector *text = object->scrollbox.text_content[i];

		ScrollboxText company_abr = {100, 5, NULL, 40, GetCompanyAbbreviation(i + 1)};
		Vector_Pushback(text, &company_abr);

		ScrollboxText company_name = {100, 40, NULL, 24, GetCompanyName(i + 1)};
		Vector_Pushback(text, &company_name);

	}

}

void SetScrollboxIconsForCompanies(DrawObject *object)
{

    int num_companies = GetNumCompanies();
	for (size_t i = 0; i < num_companies;i++) {

		Vector *icons = object->scrollbox.icons[i];

		ScrollboxIcon icon = {10, 5, GetCompanyIconPath(i + 1), NULL};
		Vector_Pushback(icons, &icon);

	}

}

DrawObject *GetCompaniesScrollbox()
{

    DrawObject *object = CreateScrollBoxObject();

    object->x          = 0;
    object->y          = 0;
    object->width      = 288;
    object->height     = 803;
    object->asset_path = "assets/images/stocksmenu/stocksmenuassets/StocksBox.png";

    unsigned int num_companies = GetNumCompanies();

    object->scrollbox.num_items    = num_companies;
    object->scrollbox.box_click    = &AccountMenuCompanyScrollBoxClick;
    object->scrollbox.text_content = malloc(sizeof(Vector *) * num_companies);
    object->scrollbox.icons        = malloc(sizeof(Vector *) * num_companies);
    
    InitScrollboxVectors(object);
    SetScrollboxTextContentForCompanies(object);
    SetScrollboxIconsForCompanies(object);

    return object;

}