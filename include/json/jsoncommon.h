#ifndef JSONCOMMON_H
#define JSONCOMMON_H

#include "vector.h"

Vector *GetJsonFilesInDirectory(const char *path);
void SetJsonObjectFromFile(json_object **object, const char *file);

array_list *JsonObjectGetArrayList(json_object *object, const char *json_path, ...);
array_list *JsonObjectGetArray(json_object *object, const char *json_path, ...);
char *JsonObjectGetString(json_object *object, const char *json_path, ...);
int JsonObjectGetInt(json_object *object, const char *json_path, ...);
float JsonObjectGetFloat(json_object *object, const char *json_path, ...);
double JsonObjectGetDouble(json_object *object, const char *json_path, ...);

#endif