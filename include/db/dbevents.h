#ifndef DBEVENTS_H
#define DBEVENTS_H

#include "vector.h"

typedef struct Event
{

	char *event;
	int modifier_length;
	float price_modifier;

} Event;

typedef struct System_Category
{

	unsigned int category_id;
	char category_name[32];

} System_Category;


void InitializeEvents();
Event *GetRandomGlobalEvent();
Event *GetRandomCategoryEvent(int category_id);
Event *GetRandomCompanyEvent(int company_id);

int GetNumCompanyCategories();
int *GetCompanyCategoryIds();
char* GetCompanyCatergoryName(int category_id);
char* GetCompanyCategory(unsigned int category_id);

#endif