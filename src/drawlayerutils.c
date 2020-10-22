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
#include "dbevents.h"

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

    size_t num_elements = object->scrollbox.num_items;
    object->scrollbox.text_content = malloc(sizeof(Vector *) * num_elements);
    object->scrollbox.icons        = malloc(sizeof(Vector *) * num_elements);

	for (size_t i = 0; i < num_elements;i++) {

		object->scrollbox.text_content[i] = Vector_Create(sizeof(ScrollboxText), 4);
		object->scrollbox.icons[i]        = Vector_Create(sizeof(ScrollboxIcon), 4);

	}

}

void SetScrollboxTextContentForCompanies(DrawObject *object) 
{

    size_t num_companies = GetNumCompanies();
	for (size_t i = 0; i < num_companies;i++) {

		Vector *text = object->scrollbox.text_content[i];

		ScrollboxText company_abr = {100, 5, NULL, 40, GetCompanyAbbreviationRef(i + 1)};
		Vector_PushBack(text, &company_abr);

		ScrollboxText company_name = {100, 40, NULL, 24, GetCompanyNameRef(i + 1)};
		Vector_PushBack(text, &company_name);

	}

}

void SetScrollboxIconsForCompanies(DrawObject *object)
{

    size_t num_companies = GetNumCompanies();
	for (size_t i = 0; i < num_companies;i++) {

		Vector *icons = object->scrollbox.icons[i];

		ScrollboxIcon icon = {10, 5, GetCompanyIconPath(i + 1), NULL};
		Vector_PushBack(icons, &icon);

	}

}

void SetScrollboxTextContentForCategories(DrawObject *object) 
{

    size_t num_categorys = GetNumCompanyCategories();
    for (size_t i = 0; i < num_categorys; i++) {

        Vector *text = object->scrollbox.text_content[i];

        ScrollboxText category_text = {100, 5, NULL, 40, GetCompanyWithCategory(i)};
        Vector_PushBack(text, &category_text);

    }

}

void SetScrollboxTextContentForCategoriesSpecificCompanies(DrawObject *object, int cat_id) 
{

    Company *companies   = GetAllCompanies();
    size_t num_companies = GetNumCompanies();

    size_t vct_idx = 0;

    for (size_t i = 0; i < num_companies;i++) {

        if(companies[i].category_id == (unsigned)cat_id) {

            Vector *text = object->scrollbox.text_content[vct_idx];

            ScrollboxText company_abr = {100, 5, NULL, 40, GetCompanyAbbreviationRef(i + 1)};
            Vector_PushBack(text, &company_abr);

            ScrollboxText company_name = {100, 40, NULL, 24, companies[i].company_name};
            Vector_PushBack(text, &company_name);
            vct_idx++;

        }

    }

}

void SetScrollboxIconsContentForCategoriesSpecificCompanies(DrawObject *object, int cat_id)
{

    Company *companies   = GetAllCompanies();
    size_t num_companies = GetNumCompanies();

    size_t vct_idx = 0;

    for (size_t i = 0; i < num_companies;i++) {

        if(companies[i].category_id == (unsigned)cat_id) {

            Vector *icons = object->scrollbox.icons[vct_idx];

            ScrollboxIcon icon = {10, 5, GetCompanyIconPath(i+ 1), NULL};
            Vector_PushBack(icons, &icon);
            vct_idx++;

        }

    }

}

DrawObject *CreateCategoryScrollbox(int x, int y, void (*click)(char *scroll_box_content, unsigned short int index))
{

    DrawObject *category_scrollbox = Scrollbox_Create();

    category_scrollbox->x          = x;
    category_scrollbox->y          = y;
    category_scrollbox->width      = 288;
    category_scrollbox->height     = 803;
    category_scrollbox->asset_path = "assets/images/stocksmenu/stocksmenuassets/StocksBox.png";

    unsigned int num_categorys = GetNumCompanyCategories();

    category_scrollbox->scrollbox.num_items = num_categorys;
    category_scrollbox->scrollbox.box_click = click;
    
    InitScrollboxVectors(category_scrollbox);
    SetScrollboxTextContentForCategories(category_scrollbox);
    //SetScrollboxIconsForCompanies(object); //I will eventually have Icons for the catergories

    return category_scrollbox;

}

DrawObject *CreateCategorySpecificCompaniesScrollbox(int x, int y, int cat_id, void (*click)(char *scroll_box_content, unsigned short int index))
{

    DrawObject *category_specific_companies_scrollbox = Scrollbox_Create();

    category_specific_companies_scrollbox->x          = x;
    category_specific_companies_scrollbox->y          = y;
    category_specific_companies_scrollbox->width      = 288;
    category_specific_companies_scrollbox->height     = 803;
    category_specific_companies_scrollbox->asset_path = "assets/images/stocksmenu/stocksmenuassets/StocksBox.png";

    
    Company *companies = GetAllCompanies();

    unsigned int amount_of_comapanies = 0;

    for(int i = 0; i < GetNumCompanies(); i++)
        if(companies[i].category_id == (unsigned)cat_id)
            amount_of_comapanies++;

    category_specific_companies_scrollbox->scrollbox.num_items = amount_of_comapanies;
    category_specific_companies_scrollbox->scrollbox.box_click = click;
    
    InitScrollboxVectors(category_specific_companies_scrollbox);
    SetScrollboxTextContentForCategoriesSpecificCompanies(category_specific_companies_scrollbox, cat_id);
    SetScrollboxIconsContentForCategoriesSpecificCompanies(category_specific_companies_scrollbox, cat_id);
    
    return category_specific_companies_scrollbox;

}

DrawObject *CreateCompaniesScrollbox(int x, int y, void (*click)(char *scroll_box_content, unsigned short int index))
{

    DrawObject *object = Scrollbox_Create();

    object->x          = x;
    object->y          = y;
    object->width      = 288;
    object->height     = 803;
    object->asset_path = "assets/images/stocksmenu/stocksmenuassets/StocksBox.png";

    unsigned int num_companies = GetNumCompanies();

    object->scrollbox.num_items = num_companies;
    object->scrollbox.box_click = click;
    
    InitScrollboxVectors(object);
    SetScrollboxTextContentForCompanies(object);
    SetScrollboxIconsForCompanies(object);

    return object;

}