
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
#define BUFFER_SIZE        1024

static json_object *draw_objects  = NULL;
static DrawObject *parsed_objects = NULL;
static unsigned int num_objects   = 0;

void SetParsedObjectsNull();
void ParseJsonDrawObject(array_list *objects_list);
void WithTypeSetDrawObject(char *type, int idx);
void SetCommonObjectProperties(int idx, char *path, char *child_of);
void SetMenuObject(int idx);
void SetVideoObject(int idx);
void SetButtonObject(int idx);
void SetTextObject(int idx);
void SetMenuButtonObject(int idx, int button_idx, char *child_of);
void SetMenuTextObject(int idx, int button_idx, char *child_of);

void CheckAndSetMenuButtons(int idx, char *child_of);
void CheckAndSetMenuText(int idx, char *child_of);
void CheckAndSetTextboxes(int idx, char *child_of);

Vector *GetDirectoryJsonFiles(const char *dirpath)
{

    Vector *files = Unix_GetJsonFilesInDirectory(dirpath);

    if (files == NULL)
        Log("Files vector never initialized");

    return files;


}

void SetJsonObjectsFromFiles(const char *dirpath, Vector *files)
{

    char filepath[128];
    char **filenames = (char **)files->elements;
    for (size_t i = 0; i < files->num_elements;i++) {

        memset(filepath, 0, 128);
        strcpy(filepath, dirpath);
        strcat(filepath, filenames[i]);
        filepath[127] = '\0';

        SetJsonObjectFromFile(&draw_objects, filepath);

        if (draw_objects == NULL)
            continue;

        array_list *objects_list = JsonObjectGetArrayList(draw_objects, "/Objects");
        if (objects_list == NULL)
            continue;
        else
            ParseJsonDrawObject(objects_list);

        draw_objects = NULL;

    }

}

void InitialzeDrawObjectsJson() 
{

    // TODO PARSE HOW MANY OBJECTS ARE ACTUALLY DEFINED IN JSON FILE
    parsed_objects = malloc(sizeof(DrawObject) * MAX_PARSED_OBJECTS);
    SetParsedObjectsNull();

    const char *dirpath = "assets/config/drawobjects/";
    Vector *files       = GetDirectoryJsonFiles(dirpath);

    if (files == NULL)
        return;

    SetJsonObjectsFromFiles(dirpath, files);

}

void ParseJsonDrawObject(array_list *objects_list) 
{

    for (size_t i = 0; i < objects_list->length; i++)
        WithTypeSetDrawObject(JsonObjectGetString(draw_objects, "/Objects/%d/Type", i), i);

}

void WithTypeSetDrawObject(char *type, int idx) 
{

    SetCommonObjectProperties(idx, "", NULL);
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

void SetCommonObjectProperties(int idx, char *path, char *child_of) 
{

    char base_path[128];
    if (path == NULL || strcmp(path, "") == 0)
        strcpy(base_path, "/Objects/%d/");
    else
        strcpy(base_path, path);

    char appended_path[256];

    parsed_objects[num_objects].bit_flags  = SHOULD_BE_DRAWN;
    parsed_objects[num_objects].name       = JsonObjectGetString(draw_objects, strcat(strcpy(appended_path, base_path), "Name"), idx);
    parsed_objects[num_objects].asset_path = JsonObjectGetString(draw_objects, strcat(strcpy(appended_path, base_path), "Path"), idx);
    parsed_objects[num_objects].x          = JsonObjectGetFloat(draw_objects, strcat(strcpy(appended_path, base_path), "X"), idx);
    parsed_objects[num_objects].y          = JsonObjectGetFloat(draw_objects, strcat(strcpy(appended_path, base_path), "Y"), idx);
    parsed_objects[num_objects].width      = JsonObjectGetFloat(draw_objects, strcat(strcpy(appended_path, base_path), "Width"), idx);
    parsed_objects[num_objects].height     = JsonObjectGetFloat(draw_objects, strcat(strcpy(appended_path, base_path), "Height"), idx);
    parsed_objects[num_objects].child_of   = child_of;

}

void SetMenuObject(int idx) 
{

    unsigned int menu_index          = num_objects;
    parsed_objects[num_objects].type = MENU;
    num_objects++;

    CheckAndSetMenuButtons(idx, parsed_objects[menu_index].name);
    CheckAndSetMenuText(idx, parsed_objects[menu_index].name);
    CheckAndSetTextboxes(idx, parsed_objects[menu_index].name);

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
    SetCommonObjectProperties(idx, GetFormattedBuffer(path, "/Objects/%d/Buttons/%d/", idx, button_idx), child_of);
    parsed_objects[num_objects].type = BUTTON;
    parsed_objects[num_objects].x    = JsonObjectGetFloat(draw_objects, "/Objects/%d/Buttons/%d/RX", idx, button_idx) + JsonObjectGetFloat(draw_objects, "/Objects/%d/X", idx);
    parsed_objects[num_objects].y    = JsonObjectGetFloat(draw_objects, "/Objects/%d/Buttons/%d/RY", idx, button_idx) + JsonObjectGetFloat(draw_objects, "/Objects/%d/Y", idx);
    num_objects++;

}

void CheckAndSetMenuButtons(int idx, char *child_of) 
{

    array_list *button_list = JsonObjectGetArrayList(draw_objects, "/Objects/%d/Buttons", idx);
    if (button_list == NULL)
        return;

    for (size_t i = 0; i < button_list->length;i++)
        SetMenuButtonObject(idx, i, child_of);

}

void SetTextObject(int idx) 
{


}

void SetMenuTextObject(int idx, int text_idx, char *child_of) 
{

    parsed_objects[num_objects].type             = TEXT;
    parsed_objects[num_objects].width            = 1000;
    parsed_objects[num_objects].asset_path       = JsonObjectGetString(draw_objects, "/Objects/%d/Text/%d/Path", idx, text_idx);
    parsed_objects[num_objects].name             = JsonObjectGetString(draw_objects, "/Objects/%d/Text/%d/Name", idx, text_idx);
    parsed_objects[num_objects].x                = JsonObjectGetFloat(draw_objects, "/Objects/%d/Text/%d/RX", idx, text_idx)  + JsonObjectGetFloat(draw_objects, "/Objects/%d/X", idx);
    parsed_objects[num_objects].y                = JsonObjectGetFloat(draw_objects,  "/Objects/%d/Text/%d/RY", idx, text_idx) + JsonObjectGetFloat(draw_objects, "/Objects/%d/Y", idx);
    parsed_objects[num_objects].text.font_size   = JsonObjectGetInt(draw_objects, "/Objects/%d/Text/%d/FontSize", idx, text_idx);
    parsed_objects[num_objects].text.content     = JsonObjectGetString(draw_objects,"/Objects/%d/Text/%d/Content", idx, text_idx);
    parsed_objects[num_objects].child_of         = child_of;
    parsed_objects[num_objects].bit_flags        = SHOULD_BE_DRAWN;
    parsed_objects[num_objects].text.bitmap_path = NULL;
    parsed_objects[num_objects].text.bitmap      = NULL;
    parsed_objects[num_objects].text.x_offset    = 0;
    parsed_objects[num_objects].text.y_offset    = 0;

    array_list *colors = JsonObjectGetArrayList(draw_objects, "/Objects/%d/Text/%d/Color", idx, text_idx);
    if (colors->length == 4) {

        parsed_objects[num_objects].text.r = JsonObjectGetInt(draw_objects, "/Objects/%d/Text/%d/Color/0", idx, text_idx);
        parsed_objects[num_objects].text.g = JsonObjectGetInt(draw_objects, "/Objects/%d/Text/%d/Color/1", idx, text_idx);
        parsed_objects[num_objects].text.b = JsonObjectGetInt(draw_objects, "/Objects/%d/Text/%d/Color/2", idx, text_idx);
        parsed_objects[num_objects].text.a = JsonObjectGetInt(draw_objects, "/Objects/%d/Text/%d/Color/3", idx, text_idx);

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

    array_list *text_list = JsonObjectGetArrayList(draw_objects, "/Objects/%d/Text", idx);

    if (text_list == NULL) 
        return;

    for (size_t i = 0; i < text_list->length;i++)
        SetMenuTextObject(idx, i, child_of);

}

void SetFontStyle(TextStyle **style, int idx, int textbox_idx, char *text_field)
{

    (*style) = malloc(sizeof(TextStyle));
    (*style)->font_size = JsonObjectGetInt(draw_objects, "/Objects/%d/Textbox/%d/%s/FontSize", idx, textbox_idx, text_field);
    (*style)->font_path = JsonObjectGetString(draw_objects, "/Objects/%d/Textbox/%d/%s/Path", idx, textbox_idx, text_field);

    array_list *colors = JsonObjectGetArrayList(draw_objects, "/Objects/%d/Textbox/%d/%s/Color", idx, textbox_idx, text_field);
    if (colors->length == 4) {

        (*style)->r = JsonObjectGetInt(draw_objects, "/Objects/%d/Textbox/%d/%s/Color/0", idx, textbox_idx, text_field);
        (*style)->g = JsonObjectGetInt(draw_objects, "/Objects/%d/Textbox/%d/%s/Color/1", idx, textbox_idx, text_field);
        (*style)->b = JsonObjectGetInt(draw_objects, "/Objects/%d/Textbox/%d/%s/Color/2", idx, textbox_idx, text_field);
        (*style)->a = JsonObjectGetInt(draw_objects, "/Objects/%d/Textbox/%d/%s/Color/3", idx, textbox_idx, text_field);

    } else {

        (*style)->r = 255;
        (*style)->g = 255;
        (*style)->b = 255;
        (*style)->a = 255;

    }

}

void SetMenuTextbox(int idx, int textbox_idx, char *child_of)
{

    parsed_objects[num_objects].type                        = TEXTBOX;
    parsed_objects[num_objects].asset_path                  = JsonObjectGetString(draw_objects, "/Objects/%d/Textbox/%d/Path", idx, textbox_idx);
    parsed_objects[num_objects].name                        = JsonObjectGetString(draw_objects, "/Objects/%d/Textbox/%d/Name", idx, textbox_idx);
    parsed_objects[num_objects].x                           = JsonObjectGetFloat(draw_objects, "/Objects/%d/Textbox/%d/RX", idx, textbox_idx) + JsonObjectGetFloat(draw_objects, "/Objects/%d/X", idx);
    parsed_objects[num_objects].y                           = JsonObjectGetFloat(draw_objects, "/Objects/%d/Textbox/%d/RY", idx, textbox_idx) + JsonObjectGetFloat(draw_objects, "/Objects/%d/Y", idx);
    parsed_objects[num_objects].width                       = JsonObjectGetFloat(draw_objects, "/Objects/%d/Textbox/%d/Width", idx, textbox_idx);
    parsed_objects[num_objects].height                      = JsonObjectGetFloat(draw_objects, "/Objects/%d/Textbox/%d/Height", idx, textbox_idx);
    parsed_objects[num_objects].textbox.limit_characters_to = JsonObjectGetInt(draw_objects, "/Objects/%d/Textbox/%d/Text/CharacterLimit", idx, textbox_idx);
    parsed_objects[num_objects].textbox.placeholder_text    = JsonObjectGetString(draw_objects, "/Objects/%d/Textbox/%d/Placeholder/Content", idx, textbox_idx);
    parsed_objects[num_objects].child_of                    = child_of;
    parsed_objects[num_objects].bit_flags                   = SHOULD_BE_DRAWN;
    parsed_objects[num_objects].bit_flags                  |= strcmp(JsonObjectGetString(draw_objects, "/Objects/%d/Textbox/%d/Text/AcceptAlphabetCharacters", idx, textbox_idx), "true") == 0 ? TEXTBOX_ACCEPT_ALPHABET_CHARACTERS : 0;
    parsed_objects[num_objects].bit_flags                  |= strcmp(JsonObjectGetString(draw_objects, "/Objects/%d/Textbox/%d/Text/AcceptNumberCharacters", idx, textbox_idx), "true") == 0 ? TEXTBOX_ACCEPT_NUMBER_CHARACTERS : 0;

    SetFontStyle(&parsed_objects[num_objects].textbox.text_style, idx, textbox_idx, "Text");
    SetFontStyle(&parsed_objects[num_objects].textbox.placeholder_style, idx, textbox_idx, "Placeholder");

    num_objects++;

}

void CheckAndSetTextboxes(int idx, char *child_of) 
{

    array_list *text_list = JsonObjectGetArrayList(draw_objects, "/Objects/%d/Textbox", idx);

    if (text_list == NULL)
        return;

    for (size_t i = 0; i < text_list->length;i++)
        SetMenuTextbox(idx, i, child_of);

}

void CleanUpDrawObjectJson() 
{

    if (parsed_objects != NULL)
        free(parsed_objects);
    parsed_objects = NULL;

}

void SetCommonDrawObjectPropertiesForGetDrawObject(DrawObject *draw_object, int object_idx) 
{

    draw_object->name       = parsed_objects[object_idx].name;
    draw_object->x          = parsed_objects[object_idx].x;
    draw_object->y          = parsed_objects[object_idx].y;
    draw_object->width      = parsed_objects[object_idx].width;
    draw_object->height     = parsed_objects[object_idx].height;
    draw_object->asset_path = parsed_objects[object_idx].asset_path;
    draw_object->bit_flags  = parsed_objects[object_idx].bit_flags;
    draw_object->child_of   = parsed_objects[object_idx].child_of;

}

void SetVideoDrawObjectFromJson(DrawObject *draw_object, int object_idx) 
{

    draw_object->type = VIDEO;

}

void SetButtonDrawObjectFromJson(DrawObject *draw_object, int object_idx) 
{

    draw_object->type            = BUTTON;
    draw_object->button.Callback = GetButtonCallback(draw_object->name);

}

void SetMenuDrawObjectFromJson(DrawObject *draw_object, int object_idx) 
{

    draw_object->type = MENU;

}

void SetTextDrawObjectFromJson(DrawObject *draw_object, int object_idx) 
{

    draw_object->type             = TEXT;
    draw_object->text.font_size   = parsed_objects[object_idx].text.font_size;
    draw_object->text.content     = parsed_objects[object_idx].text.content;
    draw_object->text.r           = parsed_objects[object_idx].text.r;
    draw_object->text.g           = parsed_objects[object_idx].text.g;
    draw_object->text.b           = parsed_objects[object_idx].text.b;
    draw_object->text.a           = parsed_objects[object_idx].text.a;
    draw_object->text.bitmap_path = NULL;
    draw_object->text.bitmap      = NULL;
    draw_object->text.x_offset    = 0;
    draw_object->text.y_offset    = 0;

}

void SetTextboxDrawObjectFromJson(DrawObject *draw_object, int object_idx)
{

    draw_object->type                        = TEXTBOX;
    draw_object->textbox.placeholder_text    = parsed_objects[object_idx].textbox.placeholder_text;
    draw_object->textbox.current_character   = -1;
    draw_object->textbox.limit_characters_to = parsed_objects[object_idx].textbox.limit_characters_to;
    draw_object->textbox.placeholder_text    = parsed_objects[object_idx].textbox.placeholder_text;
    draw_object->textbox.text_style          = parsed_objects[object_idx].textbox.text_style;
    draw_object->textbox.placeholder_style   = parsed_objects[object_idx].textbox.placeholder_style;

}

void JsonSetDrawObject(DrawObject *draw_object, int object_idx, void (*SetDrawObject)(DrawObject *, int))
{

    SetCommonDrawObjectPropertiesForGetDrawObject(draw_object, object_idx);
    SetDrawObject(draw_object, object_idx);

}

DrawObject *CreateDrawObjectFromJson(int object_idx) 
{

    DrawObject *draw_object = CreateNewDrawObject();
    switch (parsed_objects[object_idx].type) {

        case VIDEO:   JsonSetDrawObject(draw_object, object_idx, &SetVideoDrawObjectFromJson);   break;
        case BUTTON:  JsonSetDrawObject(draw_object, object_idx, &SetButtonDrawObjectFromJson);  break;
        case MENU:    JsonSetDrawObject(draw_object, object_idx, &SetMenuDrawObjectFromJson);    break;
        case TEXT:    JsonSetDrawObject(draw_object, object_idx, &SetTextDrawObjectFromJson);    break;
        case TEXTBOX: JsonSetDrawObject(draw_object, object_idx, &SetTextboxDrawObjectFromJson); break;

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

void ReclaimUnsuedSpace(unsigned char num_objects, DrawObject ***objects) 
{

    if (num_objects == 0) {

        free((*objects));
        (*objects) = NULL;

    } else {

        (*objects) = realloc((*objects), sizeof(DrawObject *) * num_objects);

    } 

}

void ReclaimUnusedSpaceFromMenuWithChilds(MenuWithChilds *menu_with_childs) 
{

    ReclaimUnsuedSpace(menu_with_childs->num_buttons, &menu_with_childs->buttons);
    ReclaimUnsuedSpace(menu_with_childs->num_text, &menu_with_childs->text);
    ReclaimUnsuedSpace(menu_with_childs->num_text_boxes, &menu_with_childs->text_boxes);

}

void SetChild(int object_idx, unsigned char *num_buttons, DrawObject **objects)
{

    if (*num_buttons == 255)
        return;

    objects[*num_buttons] = CreateDrawObjectFromJson(object_idx);
    (*num_buttons)++;

}

void SetMenuChild(int object_idx, MenuWithChilds *menu_with_childs) 
{

    switch (parsed_objects[object_idx].type) {

        case BUTTON:  SetChild(object_idx, &menu_with_childs->num_buttons, menu_with_childs->buttons); break;
        case TEXT:    SetChild(object_idx, &menu_with_childs->num_text, menu_with_childs->text); break;
        case TEXTBOX: SetChild(object_idx, &menu_with_childs->num_text_boxes, menu_with_childs->text_boxes); break;

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

    MenuWithChilds *menu_with_childs   = malloc(sizeof(MenuWithChilds));
    menu_with_childs->menu             = menu;
    menu_with_childs->num_buttons      = 0;
    menu_with_childs->num_text         = 0;
    menu_with_childs->num_text_boxes   = 0;
    menu_with_childs->num_scroll_boxes = 0;
    menu_with_childs->buttons          = malloc(sizeof(DrawObject *) * 256);
    menu_with_childs->text             = malloc(sizeof(DrawObject *) * 256);
    menu_with_childs->text_boxes       = malloc(sizeof(DrawObject *) * 256);
    menu_with_childs->scroll_boxes     = NULL;

    SetAllMenuChilds(menu_with_childs);
    ReclaimUnusedSpaceFromMenuWithChilds(menu_with_childs);

    return menu_with_childs;

}