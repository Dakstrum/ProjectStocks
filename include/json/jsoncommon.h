#ifndef JSONCOMMON_H
#define JSONCOMMON_H

void SetJsonObjectFromFile(json_object **object, const char *file);

array_list *GetArrayList(json_object *object, const char *json_path);
array_list *GetJsonObjectArray(json_object *object, const char *json_path);
char* GetStringFromJsonObject(json_object *object, const char *json_path);
int GetIntFromJsonObject(json_object *object, const char *json_path);
float GetFloatFromJsonObject(json_object *object, const char *json_path);
double GetDoubleFromJsonObject(json_object *object, const char *json_path);

#endif