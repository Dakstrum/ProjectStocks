
#include <string.h>
#include <stdlib.h>
#include <assert.h>


#include "vector.h"
#include "dbutils.h"
#include "dbevents.h"
#include "dbcompany.h"

typedef struct Events
{

	unsigned int *id;
	int num_elements;
	Vector **events;

} Events;

static Vector *global_events = NULL;
static Events category_events;
static Events company_events;


int Events_Callback(void *events, int argc, char **argv, char **col_name)
{

	if (argc == 0)
		return 0;

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

	ExecuteQueryF(&Events_Callback, &company_events, "SELECT CompanyId, Event, PriceModifier, ModifierLength FROM CompanyEvents;");

}

void InitializeCategoryEvents()
{

	int num_categories = 0;
	ExecuteQueryF(&NumCategories_Callback, &num_categories, "SELECT COUNT(CategoryName) FROM Category");

	assert(num_categories > 0);

	category_events.num_elements = num_categories;
	category_events.id           = malloc(sizeof(unsigned int) * num_categories);
	category_events.events       = malloc(sizeof(Vector *) * num_categories);

	ExecuteQueryF(&Events_Callback, &category_events, "SELECT CategoryId, Event, PriceModifier, ModifierLength FROM CategoryEvents");

}

void InitializeGlobalEvents()
{

	global_events = Vector_Create(sizeof(Event), 16);

	ExecuteQueryF(&GlobalEvents_Callback, NULL, "SELECT Event, PriceModifier, ModifierLength FROM GlobalEvents");

}

void InitializeEvents()
{

	InitializeCompanyEvents();
	InitializeCategoryEvents();
	InitializeGlobalEvents();

}