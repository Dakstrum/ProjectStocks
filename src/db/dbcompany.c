
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "shared.h"
#include "dbutils.h"
#include "dbcompany.h"

static Vector *companies = NULL;

Company *GetAllCompanies()
{

    assert(companies != NULL);
    return (Company *)companies->elements;

}

char *GetCompanyName(unsigned int company_id)
{

    Company *temp = (Company *)companies->elements;
    for (size_t i = 0; i < companies->num_elements;i++)
        if (temp[i].company_id == company_id)
            return strdup(temp[i].company_name);

    return NULL;

}

char *GetCompanyAbbreviation(unsigned int company_id)
{

    Company *temp = (Company *)companies->elements;
    for (size_t i = 0; i < companies->num_elements;i++)
        if (temp[i].company_id == company_id)
            return strdup(temp[i].company_abbreviation);

    return NULL;

}

unsigned int GetCompanyId(const char *company_name)
{

    assert(company_name != NULL);

    Company *temp = (Company *)companies->elements;
    for (size_t i = 0; i < companies->num_elements;i++)
        if (strcmp(temp[i].company_name, company_name) == 0)
            return temp[i].company_id;

    return 0;

}


int GetNumCompanies()
{

    assert(companies != NULL);
    return companies->num_elements;

}

int Company_Callback(void *company, int argc, char **argv, char **col_name) 
{

    if (argc == 0)
        return 0;

    Company temp;

    temp.company_id = atoi(argv[0]);
    strncpy(temp.company_name, argv[1], 64);

    strncpy(temp.company_abbreviation, argv[2], 6);
    temp.ipo         = atof(argv[3]);
    temp.category_id = atoi(argv[4]);

    Vector_PushBack(companies, &temp);

    return 0;

}

void InitializeCompanies()
{

    companies = Vector_Create(sizeof(Company), 4);
    ExecuteQueryF(&Company_Callback, NULL, "SELECT C.CompanyId, C.CompanyName, C.CompanyAbbreviation, C.IPO, C.CategoryId FROM Company C");

}
