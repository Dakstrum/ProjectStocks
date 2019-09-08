
#include <stdbool.h>

#include <json-c/json.h>
#include <json-c/json_util.h>
#include <json-c/json_object.h>

#include "log.h"
#include "shared.h"
#include "jsoncommon.h"
#include "drawlayers.h"

#define MAX_PARSED_OBJECTS 1024

static json_object *draw_objects  = NULL;
static DrawObject *parsed_objects = NULL;
static unsigned int num_objects   = 0;

void ParseJsonDrawObject(array_list *objects_list);
void WithTypeSetDrawObject(char *type, int idx);
void SetCommonObjectProperties(int idx, char *path);
void SetMenuObject(int idx);
void SetVideoObject(int idx);
void SetButtonObject(int idx);
void SetTextObject(int idx);
void SetMenuButtonObject(int idx, int button_idx, char *child_of);
void SetMenuTextObject(int idx, int button_idx, char *child_of);

void CheckAndSetMenuButtons(int idx, char *child_of);
void CheckAndSetMenuText(int idx, char *child_of);

void InitialzeDrawObjectsJson() 
{

    SetJsonObjectFromFile(&draw_objects, "assets/config/drawobjects.json");

    if (draw_objects == NULL)
        return;

    array_list *objects_list = GetArrayList(draw_objects, "/Objects");
    if (objects_list == NULL)
        return;
    else
       ParseJsonDrawObject(objects_list);

}

void ParseJsonDrawObject(array_list *objects_list) 
{

    // TODO PARSE HOW MANY OBJECTS ARE ACTUALLY DEFINED IN JSON FILE
    parsed_objects = malloc(sizeof(DrawObject) * MAX_PARSED_OBJECTS);

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

    CheckAndSetMenuButtons(idx, parsed_objects[num_objects-1].name);
    CheckAndSetMenuText(idx, parsed_objects[num_objects-1].name);

}

void SetVideoObject(int idx) 
{

    parsed_objects[num_objects].type = VIDEO;
    num_objects++;

}

void SetButtonObject(int idx) 
{


}

void SetMenuButtonObject(int idx, int button_idx, char *child_of) 
{

    char path[128];
    SetCommonObjectProperties(idx, GetFormattedBuffer(path, "/Objects/%d/Buttons/%d/", idx, button_idx));

    // TODO Set button callback. Requires existing function list to implement.
    parsed_objects[num_objects].type     = BUTTON;
    parsed_objects[num_objects].x        = GetFloatFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Buttons/%d/RX", idx, button_idx)) + GetFloatFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/X", idx));
    parsed_objects[num_objects].y        = GetFloatFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Buttons/%d/RY", idx, button_idx)) + GetFloatFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Y", idx));
    parsed_objects[num_objects].child_of = child_of;

    num_objects++;

}

void CheckAndSetMenuButtons(int idx, char *child_of) 
{

    char buffer[512];
    array_list *button_list = GetArrayList(draw_objects, GetFormattedBuffer(buffer, "/Objects/%d/Buttons", idx));

    if (button_list == NULL)
        return;

    for (int i = 0; i < button_list->length;i++)
        SetMenuButtonObject(idx, i, child_of);

}

void SetTextObject(int idx) 
{


}

void SetMenuTextObject(int idx, int text_idx, char *child_of) 
{

    char path[128];
    parsed_objects[num_objects].type                  = TEXT;
    parsed_objects[num_objects].name                  = GetStringFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Text/%d/Name", idx, text_idx));
    parsed_objects[num_objects].x                     = GetFloatFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Text/%d/RX", idx, text_idx)) + GetFloatFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/X", idx));
    parsed_objects[num_objects].y                     = GetFloatFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Text/%d/RY", idx, text_idx)) + GetFloatFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Y", idx));
    parsed_objects[num_objects].member.text.font_size = GetIntFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Text/%d/FontSize", idx, text_idx));
    parsed_objects[num_objects].member.text.content   = GetStringFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Text/%d/Content", idx, text_idx));
    parsed_objects[num_objects].child_of              = child_of;

    array_list *colors = GetArrayList(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Text/%d/Color",idx, text_idx));
    if (colors->length == 4) {

        parsed_objects[num_objects].member.text.r = GetIntFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Text/%d/Color/0", idx, text_idx));
        parsed_objects[num_objects].member.text.g = GetIntFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Text/%d/Color/1", idx, text_idx));
        parsed_objects[num_objects].member.text.b = GetIntFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Text/%d/Color/2", idx, text_idx));
        parsed_objects[num_objects].member.text.a = GetIntFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Text/%d/Color/3", idx, text_idx));

    } else {

        LogF("Setting Default Colors for /Objects/%d/Text/%d", idx, text_idx);
        parsed_objects[num_objects].member.text.r = 255;
        parsed_objects[num_objects].member.text.g = 255;
        parsed_objects[num_objects].member.text.b = 255;
        parsed_objects[num_objects].member.text.a = 255;

    }

    num_objects++;

}

void CheckAndSetMenuText(int idx, char *child_of) 
{

    char buffer[512];
    array_list *text_list = GetArrayList(draw_objects, GetFormattedBuffer(buffer, "/Objects/%d/Text", idx));

    if (text_list == NULL)
        return;

    for (int i = 0; i < text_list->length;i++)
        SetMenuTextObject(idx, i, child_of);

}

void CleanUpDrawObjectJson() 
{

    free(parsed_objects);

}

void SetCommonDrawObjectPropertiesForGetDrawObject(DrawObject *draw_object, int object_idx) 
{

    draw_object->x                    = parsed_objects[object_idx].x;
    draw_object->y                    = parsed_objects[object_idx].y;
    draw_object->width                = parsed_objects[object_idx].width;
    draw_object->height               = parsed_objects[object_idx].height;
    draw_object->asset_path           = parsed_objects[object_idx].asset_path;
    draw_object->should_this_be_drawn = parsed_objects[object_idx].should_this_be_drawn;

}

void SetVideoDrawObjectFromJson(DrawObject *draw_object, int object_idx) 
{

    draw_object->type = VIDEO;
    SetCommonDrawObjectPropertiesForGetDrawObject(draw_object, object_idx);

}

void SetButtonDrawObjectFromJson(DrawObject *draw_object, int object_idx) 
{


}

void SetMenuDrawObjectFromJson(DrawObject *draw_object, int object_idx) 
{

    draw_object->type = MENU;
    SetCommonDrawObjectPropertiesForGetDrawObject(draw_object, object_idx);

}

void SetTextDrawObjectFromJson(DrawObject *draw_object, int object_idx) 
{

    draw_object->type = TEXT;
    //TODO here

}

DrawObject *CreateDrawObjectFromJson(int object_idx) 
{

    DrawObject *draw_object = CreateNewDrawObject();

    switch (parsed_objects[object_idx].type) {

        case VIDEO:  SetVideoDrawObjectFromJson(draw_object, object_idx); break;
        case BUTTON: SetButtonDrawObjectFromJson(draw_object, object_idx); break;
        case MENU:   SetMenuDrawObjectFromJson(draw_object, object_idx); break;
        case TEXT:   SetTextDrawObjectFromJson(draw_object, object_idx); break;

    }

    return draw_object;

}

DrawObject *GetDrawObjectFromDrawObjectJson(char *object_name) 
{

    for (int i = 0; i < MAX_PARSED_OBJECTS; i++)
        if (parsed_objects[i].name != NULL && strcmp(object_name, parsed_objects[i].name) == 0)
            return CreateDrawObjectFromJson(i);
    
    return NULL;

}