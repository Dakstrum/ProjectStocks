#ifndef DBCOMPANY_H
#define DBCOMPANY_H

#include "vector.h"

typedef struct Company {

    int company_id;
    char *company_name;
    char *category;
    float ipo;

} Company;

Company *GetAllCompanies();
int GetNumCompanies();

void InitializeCompanies();

#endif