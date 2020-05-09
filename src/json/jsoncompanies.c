
#include <stdbool.h>

#include <json-c/json.h>
#include <json-c/json_util.h>
#include <json-c/json_object.h>

#include "log.h"
#include "shared.h"
#include "dbaccount.h"
#include "jsoncommon.h"
#include "jsoncompanies.h"

static json_object *companies     = NULL;
static Company *parsed_companies  = NULL;
static unsigned int num_companies = 0;

void InitializeCompaniesJson() 
{

    SetJsonObjectFromFile(&companies , "assets/config/companies.json");

    if (companies == NULL)
        return;

    array_list *companies_list = JsonObjectGetArrayList(companies, "/Companies");
    if (companies_list == NULL)
        return;
    else
        ParseCompanyJsonObject(companies_list);

}

void LogCompanyValues(int company_index) 
{
    Company c = parsed_companies[company_index];
}

void SetCompanyProducts(int company_index, array_list *products) 
{

    if (products == NULL)
        return;

    parsed_companies[company_index].products       = malloc(sizeof(char *) * products->length);
    parsed_companies[company_index].total_products = products->length;
    for (size_t i = 0; i < products->length; i++)
        parsed_companies[company_index].products[i] = JsonObjectGetString(companies, "/Companies/%d/Products/%d/ProductName", company_index, i);

}

void ParseCompanyJsonObject(array_list *companies_list) 
{

    num_companies    = companies_list->length;
    parsed_companies = malloc(sizeof(Company) * companies_list->length);
    for (size_t i = 0; i < companies_list->length; i++) {

        parsed_companies[i].company_name = JsonObjectGetString(companies, "/Companies/%d/CompanyName", i);
        parsed_companies[i].ipo          = JsonObjectGetFloat(companies, "/Companies/%d/IPO", i);
        parsed_companies[i].category     = JsonObjectGetString(companies, "/Companies/%d/Category", i);
        parsed_companies[i].description  = JsonObjectGetString(companies, "/Companies/%d/Description", i);
        parsed_companies[i].start_date   = JsonObjectGetString(companies, "/Companies/%d/StartDate", i);
        SetCompanyProducts(i, JsonObjectGetArray(companies, "/Companies/%d/Products", i));
        parsed_companies[i].company_id   = InsertAndOrSetCompanyToActive(parsed_companies[i].company_name, parsed_companies[i].ipo);

    }

}

void CleanUpComponaniesJson() 
{
    if (parsed_companies != NULL)
        free(parsed_companies);
    parsed_companies = NULL;

}

Company *GetAllCompanies() 
{

    return parsed_companies;

}

unsigned int GetNumCompanies() 
{

    return num_companies;

}