
#include <json-c/json.h>
#include <json-c/json_util.h>
#include <json-c/json_object.h>
#include <allegro5/allegro.h>

#include "log.h"
#include "shared.h"
#include "jsonlayer.h"

void SetJsonObjectFromFile(json_object *object, const char *file);
void ParseCompanyJsonObject();
void ParseJsonDrawObject();

array_list *GetJsonObjectArray(json_object *object, const char *json_path);

json_object *companies    = NULL;
json_object *draw_objects = NULL;

void *JsonEntry(ALLEGRO_THREAD *thread, void *arg) 
{

    SetJsonObjectFromFile(companies   , "assets/config/companies.json");
    SetJsonObjectFromFile(draw_objects, "assets/config/drawobjects.json");
    ParseCompanyJsonObject();
    ParseJsonDrawObject();

}

void SetJsonObjectFromFile(json_object *object, const char *file) 
{

    object = json_object_from_file(file);
    if (object == -1) {

        LogF("Error %s", json_util_get_last_err());
        SetCleanUpToTrue();

    } else {

        LogF("From %s retrieved %s", file, json_object_to_json_string(object));
        
    }

}

void ParseCompanyJsonObject() 
{

    if (companies == -1)
        return;

    array_list *companies_list = GetJsonObjectArray(companies, "/Companies");

    if (companies_list == NULL) {

        SetCleanUpToTrue();

    }

}

void ParseJsonDrawObject() 
{

    if (draw_objects == -1)
        return;

}

array_list *GetJsonObjectArray(json_object *object, const char *json_path) 
{

    json_object *store_object = NULL;
    int code                  = json_pointer_get(object, json_path, &store_object);

    if (code != 0)
        LogF("Unable to find array at path %s", json_path);
    else
        return json_object_get_array(store_object);

    return NULL;

}