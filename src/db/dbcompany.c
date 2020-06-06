
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
    for (int i = 0; i < companies->num_elements;i++)
        if (temp[i].company_id == company_id)
            return strdup(temp[i].company_name);

    return NULL;

}

unsigned int GetCompanyId(const char *company_name)
{

    Company *temp = (Company *)companies->elements;
    for (int i = 0; i < companies->num_elements;i++)
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

    temp.ipo = atof(argv[2]);
    strncpy(temp.category, argv[3], 32);

    PushBack(companies, &temp);

    return 0;

}

void InitializeCompanies()
{

    companies = CreateVector(sizeof(Company), 4);
    ExecuteQueryF(&Company_Callback, NULL, "SELECT C.CompanyId, C.CompanyName, C.IPO, CA.CategoryName FROM Company C INNER JOIN Category CA ON CA.CategoryId = C.CategoryId");

}
