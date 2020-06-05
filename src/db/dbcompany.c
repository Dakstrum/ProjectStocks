
#include "dbaccess.h"
#include "dbcompany.h"

static Vector *companies = NULL;

Company *GetAllCompanies()
{

    return (Company *)companies->elements;

}

int GetNumCompanies()
{

    return companies->num_elements;

}

int Company_Callback(void *company, int argc, char **argv, char **col_name) 
{

    if (argc > 0)
        return 0;

    Company temp;
    temp.company_id   = atoi(argv[0]);
    temp.company_name = argv[1];
    temp.ipo          = atof(argv[2]);
    temp.category     = argv[3];

    PushBack(companies, &temp);

    return 0;

}

void InitializeCompanies()
{

    companies = CreateVector(sizeof(Company), 4);
    ExecuteQueryF(Company_Callback, NULL, "SELECT C.CompanyId, C.CompanyName, C.IPO, CA.CategoryName FROM Company C INNER JOIN Category CA ON CA.CategoryId = C.CategoryId");

}
