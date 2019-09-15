#ifndef JSONCOMPANIES_H
#define JSONCOMPANIES_H

#include <json-c/json.h>

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

void InitializeCompaniesJson();
void ParseCompanyJsonObject(array_list *companies_list);
void ParseJsonDrawObject(array_list *objects_list);
void CleanUpComponaniesJson();

Company *GetAllCompanies();
unsigned int GetNumCompanies();

#endif