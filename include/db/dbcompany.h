#ifndef DBCOMPANY_H
#define DBCOMPANY_H

#include "vector.h"

typedef struct Company {

    int company_id;
    float ipo;
    char company_name[64];
    char category[32];

} Company;

Company *GetAllCompanies();
char *GetCompanyName(unsigned int company_id);
int GetNumCompanies();

void InitializeCompanies();

#endif