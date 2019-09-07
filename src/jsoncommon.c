#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <json-c/json.h>
#include <json-c/json_util.h>
#include <json-c/json_object.h>

#include "log.h"
#include "shared.h"

array_list *GetJsonObjectArray(json_object *object, const char *json_path);
double GetDoubleFromJsonObject(json_object *object, const char *json_path);

void SetJsonObjectFromFile(json_object **object, const char *file) 
{

    *object = json_object_from_file(file);
    if (*object == NULL) {

        LogF("Error %s", json_util_get_last_err());
        SetCleanUpToTrue();

    }

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

int GetIntFromJsonObject(json_object *object, const char *json_path) 
{

    return (int)GetDoubleFromJsonObject(object, json_path);

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