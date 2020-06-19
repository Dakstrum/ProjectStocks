#ifndef DBEVENTS_H
#define DBEVENTS_H

#include "vector.h"

typedef struct Event
{

	char *event;
	int modifier_length;
	float price_modifier;

} Event;


void InitializeEvents();
Event *GetRandomGlobalEvent();
Event *GetRandomCategoryEvent(int category_id);
Event *GetRandomCompanyEvent(int company_id);

int GetNumCompanyCategories();
int *GetCompanyCategoryIds();

#endif