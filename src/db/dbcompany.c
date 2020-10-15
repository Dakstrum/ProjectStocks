
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "shared.h"
#include "dbutils.h"
#include "dbcompany.h"
#include "log.h"

typedef struct CompanyIconPath
{
    unsigned int company_id;
    char company_icon_path[128];

} CompanyIconPath;

static Vector *companies     = NULL;
static Vector *company_icons = NULL;

Company *GetAllCompanies()
{

    assert(companies != NULL);
    return (Company *)companies->elements;

}

Company *GetCompany(unsigned int company_id)
{

    Company *temp = (Company *)companies->elements;
    for (size_t i = 0; i < companies->num_elements;i++)
        if (temp[i].company_id == company_id)
            return &temp[i];

    return NULL;

}

char *GetCompanyName(unsigned int company_id)
{

    char *company_name = GetCompanyNameRef(company_id);
    assert(company_name != NULL);
    return strdup(company_name);

}

char *GetCompanyNameRef(unsigned int company_id)
{

    Company *temp = GetCompany(company_id);
    assert(temp != NULL);
    return temp->company_name;


}

char *GetCompanyAbbreviation(unsigned int company_id)
{

    char *company_abbr = GetCompanyAbbreviationRef(company_id);
    assert(company_abbr != NULL);
    return strdup(company_abbr);

}

char *GetCompanyAbbreviationRef(unsigned int company_id)
{

    Company *temp = GetCompany(company_id);
    assert(temp != NULL);
    return temp->company_abbreviation;

}

char *GetCompanyDescriptionRef(unsigned int company_id)
{

    Company *temp = GetCompany(company_id);
    assert(temp != NULL);
    return temp->company_description;

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

char *GetCompanyIconPath(unsigned int company_id)
{

    CompanyIconPath *paths = company_icons->elements;
    for (size_t i = 0; i < company_icons->num_elements;i++)
        if (paths[i].company_id == company_id)
            return paths[i].company_icon_path;

    return NULL;

}

int Company_Callback(void *company, int argc, char **argv, char **col_name) 
{

    if (argc == 0)
        return 0;

    Company temp;

    temp.company_id = atoi(argv[0]);
    strncpy(temp.company_name,         argv[1], 64);
    strncpy(temp.company_description,  argv[2], 256);
    strncpy(temp.company_abbreviation, argv[3], 6);
    temp.ipo         = atof(argv[4]);
    temp.category_id = atoi(argv[5]);

    Vector_PushBack(companies, &temp);

    return 0;

}

int Company_Icon_Callback(void *company, int argc, char **argv, char **col_name)
{

    if (argc == 0)
        return 0;

    CompanyIconPath icon;
    icon.company_id = atoi(argv[0]);
    strncpy(icon.company_icon_path, argv[1], 128);

    Vector_PushBack(company_icons, &icon);

    return 0;

}

void InitializeCompanies()
{

    companies = Vector_Create(sizeof(Company), 4);
    company_icons = Vector_Create(sizeof(CompanyIconPath), 4);
    ExecuteQueryF(&Company_Icon_Callback, NULL, "SELECT CompanyId, IconPath FROM System_CompanyIcons");
    ExecuteQueryF(&Company_Callback, NULL, "SELECT C.CompanyId, C.CompanyName, C.CompanyDescription, C.CompanyAbbreviation, C.IPO, C.CategoryId FROM System_Company C");

}
