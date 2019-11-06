
#include <stdbool.h>

#include <json-c/json.h>
#include <json-c/json_util.h>
#include <json-c/json_object.h>

#include "log.h"
#include "shared.h"
#include "dbaccess.h"
#include "jsoncommon.h"
#include "jsoncompanies.h"

static json_object *companies     = NULL;
static Company *parsed_companies  = NULL;
static unsigned int num_companies = 0;

void InitializeCompaniesJson() 
{

    SetJsonObjectFromFile(&companies   , "assets/config/companies.json");

    if (companies == NULL)
        return;

    array_list *companies_list = GetArrayList(companies, "/Companies");
    if (companies_list == NULL)
        return;
    else
        ParseCompanyJsonObject(companies_list);

}

void LogCompanyValues(int company_index) 
{
    
    Company c = parsed_companies[company_index];
    LogF("Retrieved CompanyName = %s, IPO = %lf, Category = %s, Description = %s, StartDate = %s, TotalProducts = %d", c.company_name, c.ipo, c.category, c.description, c.start_date, c.total_products);

}

void SetCompanyProducts(int company_index, array_list *products) 
{

    if (products == NULL)
        return;

    char buffer[512];
    parsed_companies[company_index].products       = malloc(sizeof(char *) * products->length);
    parsed_companies[company_index].total_products = products->length;
    for (int i = 0; i < products->length; i++) {

        parsed_companies[company_index].products[i] = GetStringFromJsonObject(companies, GetFormattedBuffer(buffer, "/Companies/%d/Products/%d/ProductName", company_index, i));

    }

}

void ParseCompanyJsonObject(array_list *companies_list) 
{

    num_companies    = companies_list->length;
    parsed_companies = malloc(sizeof(Company) * companies_list->length);
    char buffer[512];
    for (int i = 0; i < companies_list->length; i++) {

        parsed_companies[i].company_name = GetStringFromJsonObject(companies, GetFormattedBuffer(buffer, "/Companies/%d/CompanyName", i));
        parsed_companies[i].ipo          = GetFloatFromJsonObject(companies, GetFormattedBuffer(buffer, "/Companies/%d/IPO", i));
        parsed_companies[i].category     = GetStringFromJsonObject(companies, GetFormattedBuffer(buffer, "/Companies/%d/Category", i));
        parsed_companies[i].description  = GetStringFromJsonObject(companies, GetFormattedBuffer(buffer, "/Companies/%d/Description", i));
        parsed_companies[i].start_date   = GetStringFromJsonObject(companies, GetFormattedBuffer(buffer, "/Companies/%d/StartDate", i));
        SetCompanyProducts(i, GetJsonObjectArray(companies, GetFormattedBuffer(buffer, "/Companies/%d/Products", i)));
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