#ifndef DBEVENTS_H
#define DBEVENTS_H

#include "vector.h"

typedef enum EventType {

    GLOBAL,
    CATEGORY,
    COMPANY

} EventType;

typedef struct Event
{

	char *event;
    int id;
	int modifier_length;
	float price_modifier;
    EventType event_type;

} Event;

typedef struct System_Category
{

	unsigned int category_id;
	char category_name[32];

} System_Category;


void InitializeEvents();
Event *GetRandomGlobalEvent(uint16_t seed[3]);
Event *GetRandomCategoryEvent(int category_id, uint16_t seed[3]);
Event *GetRandomCompanyEvent(int company_id, uint16_t seed[3]);

int GetNumCompanyCategories();
Vector *GetCompanyCategories();
int *GetCompanyCategoryIds();
char* GetCompanyCatergoryName(int category_id);
char* GetCompanyWithCategory(unsigned int category_id);

#endif