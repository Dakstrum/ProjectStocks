
#include <string.h>
#include <stdlib.h>
#include <assert.h>


#include "log.h"
#include "vector.h"
#include "dbutils.h"
#include "dbevents.h"
#include "dbcompany.h"
#include "shared.h"

typedef struct Events
{

	int *id;
	int num_elements;
	Vector **events;

} Events;

static Vector *global_events      = NULL;
static Vector *company_categories = NULL;
static Events category_events;
static Events company_events;

static int num_categories = 0;

int Events_Callback(void *events, int argc, char **argv, char **col_name)
{

	if (argc == 0)
		return 0;

	Events *events_temp = (Events *)events;
	int id = atoi(argv[0]);

	for (int i = 0; i < events_temp->num_elements;i++) {

		if (events_temp->id[i] == id) {

			Event temp;
			temp.event = malloc(128);

			strncpy(temp.event, argv[1], 128);
			temp.price_modifier  = atof(argv[2]);
			temp.modifier_length = atoi(argv[3]);
			temp.event[127]      = '\0';

			Vector_PushBack(events_temp->events[i], &temp);
			break;

		}

	}


	return 0;
	
}

int GlobalEvents_Callback(void *events, int argc, char **argv, char **col_name)
{

	if (argc == 0)
		return 0;

	Event temp;
	temp.event = malloc(128);

	strncpy(temp.event, argv[0], 128);
	temp.price_modifier  = atof(argv[1]);
	temp.modifier_length = atoi(argv[2]);
	temp.event[127]      = '\0';

	Vector_PushBack(global_events, &temp);

	return 0;

}

int NumCategories_Callback(void *amount, int argc, char **argv, char **col_name)
{

	if (argc == 0)
		return 0;

	*((int *)amount) = atoi(argv[0]);

	return 0;

}

void InitializeCompanyEvents()
{

	int num_companies           = GetNumCompanies();
	company_events.num_elements = num_companies;
	company_events.id           = malloc(sizeof(unsigned int) * num_companies);
	company_events.events       = malloc(sizeof(Vector *) * num_companies);

	for (int i = 0; i < num_companies;i++) {

		company_events.id[i]     = i + 1;
		company_events.events[i] = Vector_Create(sizeof(Event), 16);

	}

	ExecuteQueryF(&Events_Callback, &company_events, "SELECT CompanyId, Event, PriceModifier, ModifierLength FROM System_CompanyEvents;");

}

void InitializeCategoryEvents()
{

	ExecuteQueryF(&NumCategories_Callback, &num_categories, "SELECT COUNT(CategoryName) FROM System_Category");

	category_events.num_elements = num_categories;
	category_events.id           = malloc(sizeof(unsigned int) * num_categories);
	category_events.events       = malloc(sizeof(Vector *) * num_categories);

	for (int i = 0; i < num_categories;i++) {

		category_events.id[i]     = i + 1;
		category_events.events[i] = Vector_Create(sizeof(Event), 16);

	}

	ExecuteQueryF(&Events_Callback, &category_events, "SELECT CategoryId, Event, PriceModifier, ModifierLength FROM System_CategoryEvents");

}

int SystemCategory_Callback(void *card, int argc, char **argv, char **col_name) 
{
	
    if (argc == 0)
        return 0;

    System_Category temp;

    temp.category_id = atoi(argv[0]);
    strncpy(temp.category_name, argv[1], 32);

    Vector_PushBack(company_categories, &temp);

    return 0;

}
void InitializeSystemCategory()
{

	company_categories = Vector_Create(sizeof(System_Category), 4);
	ExecuteQueryF(&SystemCategory_Callback, NULL, "SELECT C.CategoryId, C.CategoryName FROM System_Category C");

}

void InitializeGlobalEvents()
{

	global_events = Vector_Create(sizeof(Event), 16);

	ExecuteQueryF(&GlobalEvents_Callback, NULL, "SELECT Event, PriceModifier, ModifierLength FROM System_GlobalEvents");

}

void InitializeEvents()
{

	InitializeCompanyEvents();
	InitializeCategoryEvents();
	InitializeGlobalEvents();
	InitializeSystemCategory();

}

Event *GetRandomGlobalEvent()
{

	assert(global_events != NULL && global_events->num_elements > 0);

	Event *temp = (Event *)global_events->elements;
	return &temp[rand() % global_events->num_elements];

}

Event *GetRandomEvent(Vector *events)
{

	Event *temp = (Event *)events->elements;
	if (events->num_elements == 0)
		return NULL;

	return &temp[rand() % events->num_elements];

}

Event *GetRandomCategoryEvent(int category_id)
{

	return GetRandomEvent(category_events.events[category_id - 1]);

}

Event *GetRandomCompanyEvent(int company_id)
{

	return GetRandomEvent(company_events.events[company_id - 1]);

}

int GetNumCompanyCategories()
{

	return num_categories;

}

char* GetCompanyCategory(unsigned int category_id)
{

	System_Category *temp = (System_Category *)company_categories->elements;
	return temp[category_id].category_name;

}

int *GetCompanyCategoryIds()
{

	return category_events.id;

}