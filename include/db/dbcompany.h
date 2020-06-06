#ifndef DBCOMPANY_H
#define DBCOMPANY_H

#include "vector.h"

typedef struct Company {

    unsigned int company_id;
    float ipo;
    char company_name[64];
    char category[32];

} Company;

Company *GetAllCompanies();
char *GetCompanyName(unsigned int company_id);
unsigned int GetCompanyId(const char *company_name);
int GetNumCompanies();

void InitializeCompanies();

#endif