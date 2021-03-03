#ifndef DBCOMPANY_H
#define DBCOMPANY_H

#include "vector.h"

typedef struct Company {

    unsigned int company_id;
    unsigned int category_id;
    float ipo;
    char company_name[64];
    char company_description[256];
    char company_abbreviation[6];
    unsigned int company_start_time;

} Company;

Company *GetAllCompanies();
Vector *dbcompany_get_companies_vector();

char *GetCompanyName(unsigned int company_id);
char *GetCompanyNameRef(unsigned int company_id);
char *GetCompanyAbbreviation(unsigned int company_id);
char *GetCompanyAbbreviationRef(unsigned int company_id);
char *GetCompanyDescriptionRef(unsigned int company_id);
char *GetCompanyIconPath(unsigned int company_id);

unsigned int GetCompanyStartTime(unsigned int company_id);
unsigned int GetCompanyId(const char *company_name);
int GetNumCompanies();

void InitializeCompanies();

#endif