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


void dbevents_init();
Event *dbevents_get_global_event(uint16_t seed[3]);
Event *dbevents_get_category_event(int category_id, uint16_t seed[3]);
Event *dbevents_get_company_event(int company_id, uint16_t seed[3]);

int dbevents_get_num_categories();
Vector *dbevents_get_categories();
int *dbevents_get_categories_ids();
char* dbevents_get_category_name(int category_id);
char* dbevents_get_company_with_category(unsigned int category_id);

#endif