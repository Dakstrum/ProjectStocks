#ifndef JSONLAYER_H
#define JSONLAYER_H

#include "drawlayers.h"

typedef struct Company {

    char *company_name;
    char *category;
    float ipo;
    
    char *description;
    char *start_date;
    char **products;
    char **news_articles;
    char **eavesdropper_messages;

    int total_products;
    int total_news_articles;
    int total_eavesdropper_messages;

} Company;

void InitializeJson();
void CleanUpJson();

DrawObject *GetDrawObjectFromJson(char *object_name);

#endif