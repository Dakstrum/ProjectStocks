
#include <stdio.h>
#include <string.h>

#include <json-c/json.h>
#include <json-c/json_util.h>
#include <json-c/json_object.h>
#include <allegro5/allegro.h>

#include "log.h"
#include "shared.h"
#include "jsonlayer.h"
#include "drawlayers.h"

typedef struct Company {

    char *company_name;
    char *category;
    float ipo;
    
    char *description;
    char *start_date;
    char **products;
    char **news_articles;
    char **eavesdropper_messages;

    int total_products;
    int total_news_articles;
    int total_eavesdropper_messages;

} Company;

void SetJsonObjectFromFile(json_object **object, const char *file);
void ParseJsonObjects();
void ParseCompanyJsonObject(array_list *companies_list);
void ParseJsonDrawObject(array_list *objects_list);

void WithTypeSetDrawObject(char *type, int idx);
void SetCommonObjectProperties(int idx, char *path);
void SetMenuObject(int idx);
void SetVideoObject(int idx);
void SetButtonObject(int idx);
void SetTextObject(int idx);
void SetMenuButtonObject(int idx, int button_idx);
void SetMenuTextObject(int idx, int button_idx);

void CheckAndSetMenuButtons(int idx);
void CheckAndSetMenuText(int idx);

array_list *GetArrayList(json_object *object, const char *json_path);
array_list *GetJsonObjectArray(json_object *object, const char *json_path);
char* GetStringFromJsonObject(json_object *object, const char *json_path);
float GetFloatFromJsonObject(json_object *object, const char *json_path);
double GetDoubleFromJsonObject(json_object *object, const char *json_path);

static json_object *companies     = NULL;
static json_object *draw_objects  = NULL;
static Company *parsed_companies  = NULL;
static unsigned int num_companies = 0;

static DrawObject *parsed_objects = NULL;
static unsigned int num_objects   = 0;

void InitializeJson()
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

    }

}

void ParseJsonObjects() 
{

    if (companies == NULL || draw_objects == NULL)
        return;

    array_list *companies_list = GetArrayList(companies, "/Companies");
    if (companies_list == NULL)
        return;
    else
        ParseCompanyJsonObject(companies_list);

    array_list *objects_list = GetArrayList(draw_objects, "/Objects");
    if (objects_list == NULL)
        return;
    else
       ParseJsonDrawObject(objects_list);

}

array_list *GetArrayList(json_object *object, const char *json_path) 
{

    array_list *list = GetJsonObjectArray(object, json_path);

    if (list == NULL) {

        LogF("No %d found in configuration.", json_path);
        SetCleanUpToTrue();

    } else {

        LogF("Got %d %s", list->length, json_path);
        return list;
    }

    return NULL;

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
        LogCompanyValues(i);

    }

}

void ParseJsonDrawObject(array_list *objects_list) 
{

    // TODO PARSE HOW MANY OBJECTS ARE ACTUALLY DEFINED IN JSON FILE
    parsed_objects = malloc(sizeof(DrawObject) * 1024);

    char buffer[512];
    for (int i = 0; i < objects_list->length; i++)
        WithTypeSetDrawObject(GetStringFromJsonObject(draw_objects, GetFormattedBuffer(buffer, "/Objects/%d/Type", i)), i);

}

void WithTypeSetDrawObject(char *type, int idx) 
{

    SetCommonObjectProperties(idx, "");
    if (strcmp(type, "Menu") == 0)
        SetMenuObject(idx);
    else if (strcmp(type, "Video") == 0)
        SetVideoObject(idx);
    else if (strcmp(type, "Button") == 0)
        SetButtonObject(idx);
    else if (strcmp(type, "Text") == 0)
        SetTextObject(idx);
}

void SetCommonObjectProperties(int idx, char *path) 
{

    char base_path[128];
    if (path == NULL | path == "")
        strcpy(base_path, "/Objects/%d/");
    else
        strcpy(base_path, path);

    char buffer[512];
    char appended_path[256];

    parsed_objects[num_objects].should_this_be_drawn = true;
    parsed_objects[num_objects].name       = GetStringFromJsonObject(draw_objects, GetFormattedBuffer(buffer, strcat(strcpy(appended_path, base_path), "Name"), idx));
    parsed_objects[num_objects].asset_path = GetStringFromJsonObject(draw_objects, GetFormattedBuffer(buffer, strcat(strcpy(appended_path, base_path), "Path"), idx));
    parsed_objects[num_objects].x          = GetFloatFromJsonObject(draw_objects, GetFormattedBuffer(buffer, strcat(strcpy(appended_path, base_path), "X"), idx));
    parsed_objects[num_objects].y          = GetFloatFromJsonObject(draw_objects, GetFormattedBuffer(buffer, strcat(strcpy(appended_path, base_path), "Y"), idx));
    parsed_objects[num_objects].width      = GetFloatFromJsonObject(draw_objects, GetFormattedBuffer(buffer, strcat(strcpy(appended_path, base_path), "Width"), idx));
    parsed_objects[num_objects].height     = GetFloatFromJsonObject(draw_objects, GetFormattedBuffer(buffer, strcat(strcpy(appended_path, base_path), "Height"), idx));

}

void SetMenuObject(int idx) 
{

    parsed_objects[num_objects].type = MENU;
    num_objects++;

    CheckAndSetMenuButtons(idx);
    CheckAndSetMenuText(idx);

}

void SetVideoObject(int idx) 
{

    parsed_objects[num_objects].type = VIDEO;
    num_objects++;

}

void SetButtonObject(int idx) 
{


}

void SetMenuButtonObject(int idx, int button_idx) 
{

    char path[128];
    SetCommonObjectProperties(idx, GetFormattedBuffer(path, "/Objects/%d/Buttons/%d", idx, button_idx));

    // TODO Set button callback. Requires existing function list to implement.
    parsed_objects[num_objects].type = BUTTON;
    parsed_objects[num_objects].x    = GetFloatFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Buttons/%d/RX", idx, button_idx)) + GetFloatFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/X", idx));
    parsed_objects[num_objects].y    = GetFloatFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Buttons/%d/RY", idx, button_idx)) + GetFloatFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Y", idx));

    num_objects++;

}

void CheckAndSetMenuButtons(int idx) 
{

    char buffer[512];
    array_list *button_list = GetArrayList(draw_objects, GetFormattedBuffer(buffer, "/Objects/%d/Buttons", idx));

    if (button_list == NULL)
        return;

    for (int i = 0; i < button_list->length;i++)
        SetMenuButtonObject(idx, i);

}

void SetTextObject(int idx) 
{


}

void SetMenuTextObject(int idx, int button_idx) 
{



}

void CheckAndSetMenuText(int idx) 
{

    char buffer[512];
    array_list *text_list = GetArrayList(draw_objects, GetFormattedBuffer(buffer, "/Objects/%d/Text", idx));

    if (text_list == NULL)
        return;

    for (int i = 0; i < text_list->length;i++)
        SetMenuTextObject(idx, i);

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

float GetFloatFromJsonObject(json_object *object, const char *json_path) 
{

    return (float)GetDoubleFromJsonObject(object, json_path);
    
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