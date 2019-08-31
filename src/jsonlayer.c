
#include <stdio.h>

#include <json-c/json.h>
#include <json-c/json_util.h>
#include <json-c/json_object.h>
#include <allegro5/allegro.h>

#include "log.h"
#include "shared.h"
#include "jsonlayer.h"

void SetJsonObjectFromFile(json_object **object, const char *file);
void ParseJsonObjects();
void ParseCompanyJsonObject(array_list *companies_list);
void ParseJsonDrawObject();

array_list *GetCompanyArrayList();

array_list *GetJsonObjectArray(json_object *object, const char *json_path);
char* GetStringFromJsonObject(json_object *object, const char *json_path);
double GetDoubleFromJsonObject(json_object *object, const char *json_path);

static json_object *companies    = NULL;
static json_object *draw_objects = NULL;

static Company *parsed_companies = NULL;

void *JsonEntry(ALLEGRO_THREAD *thread, void *arg) 
{

    SetJsonObjectFromFile(&companies   , "assets/config/companies.json");
    SetJsonObjectFromFile(&draw_objects, "assets/config/drawobjects.json");
    GetStringFromJsonObject(companies, "/Test");
    ParseJsonObjects();

}

void SetJsonObjectFromFile(json_object **object, const char *file) 
{

    *object = json_object_from_file(file);
    if (*object == NULL) {

        LogF("Error %s", json_util_get_last_err());
        SetCleanUpToTrue();

    } else {

        LogF(json_object_to_json_string(*object));

    }

}

void ParseJsonObjects() 
{

    if (companies == NULL || draw_objects == NULL)
        return;

    array_list *companies_list = GetCompanyArrayList();

    if (companies_list == NULL)
        return;
    else
        ParseCompanyJsonObject(companies_list);

}

array_list *GetCompanyArrayList() 
{

    array_list *companies_list = GetJsonObjectArray(companies, "/Companies");

    if (companies_list == NULL) {

        Log("No companies found in configuration.");
        SetCleanUpToTrue();

    } else {

        LogF("Got %d companies", companies_list->length);
        return companies_list;
    }

    return NULL;

}

void ParseCompanyJsonObject(array_list *companies_list) 
{

    parsed_companies = malloc(sizeof(Company) * companies_list->length);
    char buffer[511];
    for (int i = 0; i < companies_list->length; i++) {

        parsed_companies[i].company_name = GetStringFromJsonObject(companies, GetFormattedBuffer(buffer, "/Companies/%d/CompanyName", i));
        parsed_companies[i].ipo          = GetDoubleFromJsonObject(companies, GetFormattedBuffer(buffer, "/Companies/%d/IPO", i));
        parsed_companies[i].category     = GetStringFromJsonObject(companies, GetFormattedBuffer(buffer, "/Companies/%d/Category", i));
        
    }

}

void ParseJsonDrawObject() 
{

    if (draw_objects == NULL)
        return;

}

array_list *GetJsonObjectArray(json_object *object, const char *json_path) 
{

    json_object *store_object = NULL;

    if (json_pointer_get(object, json_path, &store_object) != 0)
        LogF("Unable to find array at path %s", json_path);
    else
        return json_object_get_array(store_object);

    return NULL;

}

char* GetStringFromJsonObject(json_object *object, const char *json_path) 
{

    json_object *store_object = NULL;
    
    if (object == NULL)
        Log("Object is null");

    int code = json_pointer_get(object, json_path, &store_object);
    if (code != 0)
        LogF("Unable to find %s. Code = %d", json_path, code);
    else
        return strdup(json_object_get_string(store_object));

    return "";

}

double GetDoubleFromJsonObject(json_object *object, const char *json_path) 
{

    json_object *store_object = NULL;
    
    if (object == NULL)
        Log("Object is null");

    int code = json_pointer_get(object, json_path, &store_object);
    if (code != 0)
        LogF("Unable to find %s. Code = %d", json_path, code);
    else
        return json_object_get_double(store_object);

    return -1;

}

void CleanUpJson() 
{

    free(parsed_companies);

}