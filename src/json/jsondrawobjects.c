
#include <stdbool.h>

#include <json-c/json.h>
#include <json-c/json_util.h>
#include <json-c/json_object.h>

#include "log.h"
#include "shared.h"
#include "buttons.h"
#include "jsoncommon.h"
#include "drawlayers.h"

#define MAX_PARSED_OBJECTS 1024

static json_object *draw_objects  = NULL;
static DrawObject *parsed_objects = NULL;
static unsigned int num_objects   = 0;

void SetParsedObjectsNull();
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
    SetParsedObjectsNull();

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

void SetParsedObjectsNull() 
{

    for (unsigned int i = 0; i < MAX_PARSED_OBJECTS; i++) {

        parsed_objects[i].name       = NULL;
        parsed_objects[i].asset_path = NULL;
        parsed_objects[i].child_of   = NULL;

    }

}

void SetCommonObjectProperties(int idx, char *path) 
{

    char base_path[128];
    if (path == NULL || strcmp(path, "") == 0)
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

    unsigned int menu_index          = num_objects;
    parsed_objects[num_objects].type = MENU;
    num_objects++;

    CheckAndSetMenuButtons(idx, parsed_objects[menu_index].name);
    CheckAndSetMenuText(idx, parsed_objects[menu_index].name);

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
    parsed_objects[num_objects].asset_path            = GetStringFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Text/%d/Path", idx, text_idx));
    parsed_objects[num_objects].name                  = GetStringFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Text/%d/Name", idx, text_idx));
    parsed_objects[num_objects].x                     = GetFloatFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Text/%d/RX", idx, text_idx)) + GetFloatFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/X", idx));
    parsed_objects[num_objects].y                     = GetFloatFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Text/%d/RY", idx, text_idx)) + GetFloatFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Y", idx));
    parsed_objects[num_objects].text.font_size = GetIntFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Text/%d/FontSize", idx, text_idx));
    parsed_objects[num_objects].text.content   = GetStringFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Text/%d/Content", idx, text_idx));
    parsed_objects[num_objects].child_of              = child_of;
    parsed_objects[num_objects].should_this_be_drawn  = true;

    array_list *colors = GetArrayList(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Text/%d/Color",idx, text_idx));
    if (colors->length == 4) {

        parsed_objects[num_objects].text.r = GetIntFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Text/%d/Color/0", idx, text_idx));
        parsed_objects[num_objects].text.g = GetIntFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Text/%d/Color/1", idx, text_idx));
        parsed_objects[num_objects].text.b = GetIntFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Text/%d/Color/2", idx, text_idx));
        parsed_objects[num_objects].text.a = GetIntFromJsonObject(draw_objects, GetFormattedBuffer(path, "/Objects/%d/Text/%d/Color/3", idx, text_idx));

    } else {

        parsed_objects[num_objects].text.r = 255;
        parsed_objects[num_objects].text.g = 255;
        parsed_objects[num_objects].text.b = 255;
        parsed_objects[num_objects].text.a = 255;

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

    if (parsed_objects != NULL)
        free(parsed_objects);
    parsed_objects = NULL;

}

void SetCommonDrawObjectPropertiesForGetDrawObject(DrawObject *draw_object, int object_idx) 
{

    draw_object->name                 = parsed_objects[object_idx].name;
    draw_object->x                    = parsed_objects[object_idx].x;
    draw_object->y                    = parsed_objects[object_idx].y;
    draw_object->width                = parsed_objects[object_idx].width;
    draw_object->height               = parsed_objects[object_idx].height;
    draw_object->asset_path           = parsed_objects[object_idx].asset_path;
    draw_object->should_this_be_drawn = parsed_objects[object_idx].should_this_be_drawn;
    draw_object->child_of             = parsed_objects[object_idx].child_of;

}

void SetVideoDrawObjectFromJson(DrawObject *draw_object, int object_idx) 
{

    draw_object->type = VIDEO;
    SetCommonDrawObjectPropertiesForGetDrawObject(draw_object, object_idx);

}

void SetButtonDrawObjectFromJson(DrawObject *draw_object, int object_idx) 
{

    draw_object->type = BUTTON;
    SetCommonDrawObjectPropertiesForGetDrawObject(draw_object, object_idx);
    draw_object->button.Callback = GetButtonCallback(draw_object->name);

}

void SetMenuDrawObjectFromJson(DrawObject *draw_object, int object_idx) 
{

    draw_object->type = MENU;
    SetCommonDrawObjectPropertiesForGetDrawObject(draw_object, object_idx);

}

void SetTextDrawObjectFromJson(DrawObject *draw_object, int object_idx) 
{

    draw_object->type = TEXT;
    SetCommonDrawObjectPropertiesForGetDrawObject(draw_object, object_idx);

    draw_object->text.font_size = parsed_objects[object_idx].text.font_size;
    draw_object->text.content   = parsed_objects[object_idx].text.content;
    draw_object->text.r         = parsed_objects[object_idx].text.r;
    draw_object->text.g         = parsed_objects[object_idx].text.g;
    draw_object->text.b         = parsed_objects[object_idx].text.b;
    draw_object->text.a         = parsed_objects[object_idx].text.a;

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

    if (object_name == NULL)
        return NULL;

    for (int i = 0; i < MAX_PARSED_OBJECTS; i++)
        if (parsed_objects[i].name != NULL && strcmp(object_name, parsed_objects[i].name) == 0)
            return CreateDrawObjectFromJson(i);
    
    return NULL;

}

void ReclaimUnusedSpaceFromMenuWithChilds(MenuWithChilds *menu_with_childs) 
{

    if (menu_with_childs->num_buttons == 0) {

        free(menu_with_childs->buttons);
        menu_with_childs->buttons = NULL;

    } else {

        menu_with_childs->buttons = realloc(menu_with_childs->buttons, sizeof(DrawObject *) * menu_with_childs->num_buttons);

    }

    if (menu_with_childs->num_text == 0){

        free(menu_with_childs->text);
        menu_with_childs->text = NULL;

    } else {

        menu_with_childs->text = realloc(menu_with_childs->text, sizeof(DrawObject *) * menu_with_childs->num_text);

    }

}

void SetMenuChild(int object_idx, MenuWithChilds *menu_with_childs) 
{

    switch (parsed_objects[object_idx].type) {

        case BUTTON:
            if (menu_with_childs->num_buttons == 255) {

                Log("Too many child buttons in menu");
                return;

            }
            menu_with_childs->buttons[menu_with_childs->num_buttons] = CreateDrawObjectFromJson(object_idx); 
            menu_with_childs->num_buttons++; 
            break;
        case TEXT:
            if (menu_with_childs->num_text == 255) {

                Log("Too many child text in menu");
                return;

            }
            menu_with_childs->text[menu_with_childs->num_text] = CreateDrawObjectFromJson(object_idx);
            menu_with_childs->num_text++; 
            break;

    }

}

void SetAllMenuChilds(MenuWithChilds *menu_with_childs) 
{

    for (int i = 0;i < MAX_PARSED_OBJECTS;i++)
        if (parsed_objects[i].child_of != NULL && strcmp(menu_with_childs->menu->name, parsed_objects[i].child_of) == 0)
            SetMenuChild(i, menu_with_childs);

}

MenuWithChilds *GetMenuWithChildsFromDrawObjectJson(char *menu_name) 
{

    DrawObject *menu = GetDrawObjectFromDrawObjectJson(menu_name);

    if (menu == NULL)
        return NULL;

    MenuWithChilds *menu_with_childs = malloc(sizeof(MenuWithChilds));
    menu_with_childs->menu           = menu;
    menu_with_childs->num_buttons    = 0;
    menu_with_childs->num_text       = 0;
    menu_with_childs->buttons        = malloc(sizeof(DrawObject *) * 256);
    menu_with_childs->text           = malloc(sizeof(DrawObject *) * 256);

    SetAllMenuChilds(menu_with_childs);
    ReclaimUnusedSpaceFromMenuWithChilds(menu_with_childs);

    return menu_with_childs;

}