#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>

#include <json-c/json.h>
#include <json-c/json_util.h>
#include <json-c/json_object.h>

#include "log.h"
#include "shared.h"
#include "jsoncommon.h"

Vector *Unix_GetJsonFilesInDirectory(const char *path) 
{

    const int FILENAME_BUFF_SIZE = 32;

    Vector *files = Vector_Create(sizeof(char *), 8);
    DIR *dp;
    struct dirent *ep;

    dp = opendir(path);
    if (dp == NULL) {

        LogF("Unable to open directory %s", path);
        return files;

    }

    char *filename = NULL;
    while ((ep = readdir(dp)))
    {

        if (strstr(ep->d_name, ".json") == NULL)
            continue;

        filename = malloc(FILENAME_BUFF_SIZE);
        strncpy(filename, ep->d_name, FILENAME_BUFF_SIZE);
        filename[31] = '\0';
        Vector_PushBack(files, &filename);

    }
    closedir(dp);

    return files;

}

void SetJsonObjectFromFile(json_object **object, const char *file) 
{

    *object = json_object_from_file(file);
    if (*object == NULL) {

        LogF("Error %s", json_util_get_last_err());
        SetCleanUpToTrue();

    }

}

array_list *GetArray(json_object *object, const char *json_path) 
{

    json_object *store_object = NULL;

    if (json_pointer_get(object, json_path, &store_object) == 0)
        return json_object_get_array(store_object);

    return NULL;

}

array_list *GetArrayList(json_object *object, const char *json_path) 
{

    array_list *list = GetArray(object, json_path);

    if (list == NULL)
        LogF("No %d found in configuration.", json_path);
    else
        return list;

    return NULL;

}

char* GetString(json_object *object, const char *json_path) 
{

    json_object *store_object = NULL;
    
    if (object == NULL)
        Log("Object is null");

    int code = json_pointer_get(object, json_path, &store_object);
    if (code == 0)
        return strdup(json_object_get_string(store_object));
        
    return "";

}

double GetDouble(json_object *object, const char *json_path) 
{

    json_object *store_object = NULL;
    
    if (object == NULL)
        Log("Object is null");

    int code = json_pointer_get(object, json_path, &store_object);
    if (code == 0)
        return json_object_get_double(store_object);
        
    return -1;

}

int GetInt(json_object *object, const char *json_path) 
{

    return (int)GetDouble(object, json_path);

}

float GetFloat(json_object *object, const char *json_path) 
{

    return (float)GetDouble(object, json_path);

}

array_list *JsonObjectGetArrayList(json_object *object, const char *json_path, ...)
{

    va_list args;
    va_start(args, json_path);

    char *buffer      = GetFormattedPointerVaList(json_path, args);
    array_list *value = GetArrayList(object, buffer);
    free(buffer);

    return value;

}

array_list *JsonObjectGetArray(json_object *object, const char *json_path, ...)
{

    va_list args;
    va_start(args, json_path);

    char *buffer      = GetFormattedPointerVaList(json_path, args);
    array_list *value = GetArray(object, buffer);
    free(buffer);

    return value;

}

char *JsonObjectGetString(json_object *object, const char *json_path, ...)
{

    va_list args;
    va_start(args, json_path);

    char *buffer = GetFormattedPointerVaList(json_path, args);
    char *value  = GetString(object, buffer);
    free(buffer);

    return value;

}

int JsonObjectGetInt(json_object *object, const char *json_path, ...) 
{

    va_list args;
    va_start(args, json_path);

    char *buffer = GetFormattedPointerVaList(json_path, args);
    int value    = GetInt(object, buffer);
    free(buffer);

    return value;

}

float JsonObjectGetFloat(json_object *object, const char *json_path, ...)
{

    va_list args;
    va_start(args, json_path);

    char *buffer = GetFormattedPointerVaList(json_path, args);
    float value  = GetFloat(object, buffer);
    free(buffer);

    return value;

}

double JsonObjectGetDouble(json_object *object, const char *json_path, ...)
{

    va_list args;
    va_start(args, json_path);

    char *buffer = GetFormattedPointerVaList(json_path, args);
    double value = GetDouble(object, buffer);
    free(buffer);

    return value;

}