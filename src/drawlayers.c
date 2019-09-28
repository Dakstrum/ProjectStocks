#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_video.h>

#include "log.h"
#include "audio.h"
#include "cache.h"
#include "drawlayers.h"

#define MAX_DRAW_LAYERS 10
#define MAX_OBJECTS_PER_LAYER 256

typedef struct DrawLayer 
{

    DrawObject *objects[256];

} DrawLayer;


static DrawLayer *draw_layers;
static int current_draw_layer = -1;

static ALLEGRO_DISPLAY *display = NULL;
static float scale              = 1.0f;

void SetObjectPointersToNull();

void ClearUpDrawLayer(int layer);
void CleanUpButton(DrawObject *object);
void CleanUpMenu(DrawObject *object);
void CleanUpPopUp(DrawObject *object);
void CleanUpVideo(DrawObject *object);
void ClearUpGeneric(DrawObject *object);

bool HandleMouseClick(DrawObject *object, int x, int y);
bool IsMouseClickInAreaOfObject(DrawObject *object, int x, int y);

int AddDrawObjectToDrawLayer(DrawObject *object);

void DrawSingleLayer(DrawLayer *layer);
void DrawObjectOfTypeGen(DrawLayer *layer, int i);
void DrawMenu(DrawObject *object);
void DrawButton(DrawObject *object);
void DrawVideo(DrawObject *object);
void DrawText(DrawObject *object);
void DrawGeneric(ALLEGRO_BITMAP *bitmap, float x, float y);
void DrawGenericWithWidth(ALLEGRO_BITMAP *bitmap, float x, float y, float width, float height);

void InitializeDrawLayers(ALLEGRO_DISPLAY *active_display) 
{

    display     = active_display;
    draw_layers = malloc(sizeof(DrawLayer) * MAX_DRAW_LAYERS);
    current_draw_layer = -1;
    SetObjectPointersToNull();

}

void SetObjectPointersToNull() 
{

    for (int i = 0; i < MAX_DRAW_LAYERS;i++)
        for (int j = 0;j < MAX_OBJECTS_PER_LAYER; j++)
            draw_layers[i].objects[j] = NULL;

}

void HandleMouseClickInButtonAreas(int x, int y) 
{

    for (int i = 0; i < MAX_OBJECTS_PER_LAYER;i++)
        if (draw_layers[current_draw_layer].objects[i] != NULL &&  draw_layers[current_draw_layer].objects[i]->type == BUTTON)
            if (HandleMouseClick(draw_layers[current_draw_layer].objects[i], x, y))
                break;

}

bool HandleMouseClick(DrawObject *object, int x, int y) 
{

    if (IsMouseClickInAreaOfObject(object, x, y)) {

        if (object->member.button.Callback != NULL) {

            object->member.button.Callback();
            return true;

        }

    }
    return false;

}

bool IsMouseClickInAreaOfObject(DrawObject *object, int x, int y) 
{

    if (object->x > x || x > object->x + object->width)
        return false;
    if (object->y > y || y > object->y + object->height)
        return false;

    return true;

}

/* SECTION: Draw Layer Creation and Cleanup */
int CreateNewDrawLayer() 
{

    if (current_draw_layer + 1 == MAX_DRAW_LAYERS) {

        return -1;

    }
    current_draw_layer++;
    return current_draw_layer;

}

void ClearCurrentDrawLayer() 
{

    ClearUpDrawLayer(current_draw_layer);
    current_draw_layer--;

}


void ClearDrawLayers() 
{

    for (int i = 0; i < current_draw_layer; i++)
        ClearUpDrawLayer(i);

    free(draw_layers);
    InitializeDrawLayers(display);

}

void ClearUpDrawLayer(int layer) 
{

    for (int j = 0; j < MAX_OBJECTS_PER_LAYER; j++) {

        if (draw_layers[layer].objects[j] != NULL) {

            ClearUpGeneric(draw_layers[layer].objects[j]);
            free(draw_layers[layer].objects[j]);
            draw_layers[layer].objects[j] = NULL;

        }

    }

}

void ClearUpGeneric(DrawObject *object) 
{

    switch (object->type) {

        case MENU:   CleanUpMenu(object);   break;
        case BUTTON: CleanUpButton(object); break;
        case POPUP:  CleanUpPopUp(object);  break;
        case VIDEO:  CleanUpVideo(object);  break;

    }

}

void CleanUpButton(DrawObject *object) 
{

    //al_destroy_bitmap(object->member.button.button_bitmap);

}

void CleanUpMenu(DrawObject *object) 
{

    //al_destroy_bitmap(object->member.menu.menu_bitmap);

}

void CleanUpPopUp(DrawObject *object) 
{
    
    LogF("ObjectPOP= %s", object->name);
    Log("PopUp STUB");

}

void CleanUpVideo(DrawObject *object) 
{

    al_close_video(object->member.video.video);

}


/* SECTION: Add Objects to Current Draw Layer */
int AddButtonToDrawLayer(DrawObject *object) 
{

    if (object->asset_path != NULL)
        object->member.button.button_bitmap = GetBitmapFromCache(object->asset_path);

    return AddDrawObjectToDrawLayer(object);

}

int AddMenuWithChildsToDrawLayer(MenuWithChilds *menu_with_childs) 
{

    AddMenuToDrawLayer(menu_with_childs->menu);
    for (int i = 0; i < menu_with_childs->num_buttons;i++)
        AddButtonToDrawLayer(menu_with_childs->buttons[i]);
    for (int i = 0; i < menu_with_childs->num_text;i++)
        AddTextToDrawLayer(menu_with_childs->text[i]);

}

int AddObjectToDrawLayer(DrawObject *object) 
{

    switch (object->type) {

        case MENU:   return AddMenuToDrawLayer(object);   break;
        case BUTTON: return AddButtonToDrawLayer(object); break;
        case POPUP:                                       break;
        case VIDEO:  return AddVideoToDrawLayer(object);  break;
        case TEXT:   return AddTextToDrawLayer(object);   break;

    }

    return -1;

}

int AddMenuToDrawLayer(DrawObject *object) 
{

    object->member.menu.menu_bitmap = GetBitmapFromCache(object->asset_path);
    if (object->member.menu.menu_bitmap == NULL) {

        Log("Could not create menu_bitmap");
        return -1;

    }

    return AddDrawObjectToDrawLayer(object);

}

int AddVideoToDrawLayer(DrawObject *object) 
{

    object->member.video.video = al_open_video(object->asset_path);
    if (object->member.video.video == NULL) {

        LogF("Unable to open %s", object->asset_path);
        return -1;
    }
    LogF("Adding Video %s to DrawLayer", object->asset_path);
    if (object->member.video.start_video_immediately)
        al_start_video(object->member.video.video, al_get_default_mixer());

    return AddDrawObjectToDrawLayer(object);

}

int AddDrawObjectToDrawLayer(DrawObject *object) 
{

    DrawLayer *layer = &draw_layers[current_draw_layer];
    for (int i = 0; i < MAX_OBJECTS_PER_LAYER; i++) {

        if (layer->objects[i] == NULL) {

            layer->objects[i]    = object;
            object->layer_index  = current_draw_layer;
            object->object_index = i;
            return 0;

        }

    }
    return -1;
}


int AddTextToDrawLayer(DrawObject *object)
{

    object->member.text.font  = al_load_ttf_font(object->asset_path, object->member.text.font_size, 0);
    object->member.text.color = al_map_rgba(object->member.text.r, object->member.text.g, object->member.text.b, object->member.text.a);

    if (object->member.text.font == NULL) {

        LogF("Unable to open font %s", object->asset_path);
        return -1;

    }

    return AddDrawObjectToDrawLayer(object);

}


/* SECTION: Drawing functions */
void DrawLayers() 
{

    for (int i = 0; i < current_draw_layer+1;i++)
        DrawSingleLayer(&draw_layers[i]);
}

void DrawSingleLayer(DrawLayer *layer) 
{

    for (int i = 0; i < MAX_OBJECTS_PER_LAYER; i++) 
        if (layer->objects[i] != NULL && layer->objects[i]->should_this_be_drawn)
            DrawObjectOfTypeGen(layer, i);

}

void DrawObjectOfTypeGen(DrawLayer *layer, int i) 
{

    switch (layer->objects[i]->type) {

        case MENU:   DrawMenu(layer->objects[i]);   break;
        case BUTTON: DrawButton(layer->objects[i]); break;
        case POPUP:   break;
        case VIDEO:  DrawVideo(layer->objects[i]);   break;
        case TEXT:   DrawText(layer->objects[i]);    break;

    }

}

void DrawMenu(DrawObject *object) 
{

    if (object->width != 0.0f && object->height != 0.0f)
        DrawGenericWithWidth(object->member.menu.menu_bitmap, object->x, object->y, object->width, object->height);
    else
        DrawGeneric(object->member.menu.menu_bitmap, object->x, object->y);

}

void DrawButton(DrawObject *object) 
{

    DrawGeneric(object->member.button.button_bitmap, object->x, object->y);

}

void DrawVideo(DrawObject *object) 
{

    if (!al_is_video_playing(object->member.video.video))
        return;

    DrawGenericWithWidth(al_get_video_frame(object->member.video.video), object->x, object->y, object->width, object->height);

}

void DrawText(DrawObject *object) 
{

    al_draw_text(object->member.text.font, object->member.text.color, object->x, object->y, 0, object->member.text.content);

}

void DrawGeneric(ALLEGRO_BITMAP *bitmap, float x, float y) 
{

    if (bitmap == NULL)
        return;

    float scale_width  = al_get_bitmap_width(bitmap);
    float scale_height = al_get_bitmap_height(bitmap);
    al_draw_scaled_bitmap(bitmap, 0, 0, scale_width, scale_height, x, y, scale_width, scale_height, 0);

}

void DrawGenericWithWidth(ALLEGRO_BITMAP *bitmap, float x, float y, float width, float height) 
{

    if (bitmap == NULL)
        return;

    float scale_width  = al_get_bitmap_width(bitmap);
    float scale_height = al_get_bitmap_height(bitmap);
    al_draw_scaled_bitmap(bitmap, 0, 0, scale_width, scale_height, x, y, width * scale, height * scale, 0);

}

DrawObject *GetDrawObject(int layer, int object) 
{

    return draw_layers[layer].objects[object];

}

int RemoveDrawObject(DrawObject *object) 
{

    if (object == NULL) {

        Log("RemoveDrawObject, object = null");
        return -1;

    }
    if (draw_layers[object->layer_index].objects[object->object_index] == NULL) {

        LogF("object with name %s", object->name);
        return -1;

    }
    ClearUpGeneric(object);

    int layer_index  = object->layer_index;
    int object_index = object->object_index;
    free(draw_layers[layer_index].objects[object_index]);
    draw_layers[layer_index].objects[object_index] = NULL;
    
    return 0;

}

DrawObject *CreateNewDrawObject() 
{

    return malloc(sizeof(DrawObject));

}

bool DoesObjectExistInCurrentDrawLayer(char *object_name)
{

    for (int i = 0; i < MAX_OBJECTS_PER_LAYER; i++) {

        DrawObject *object = draw_layers[current_draw_layer].objects[i];

        if (object != NULL && object->name != NULL) {

            if (strcmp(object->name, object_name) == 0) {

                return true;

            }     

        }

    }

    return false;

}

bool IsThisObjectAChildOf(char *object_name, char *parent_name)
{
    for(int i = 0; i < MAX_OBJECTS_PER_LAYER; i++) {

        DrawObject *object = draw_layers[current_draw_layer].objects[i];
        if(object != NULL) {

            if(strcmp(object->name, object_name) == 0) {

                bool is_true = strcmp(object->child_of, parent_name) == 0;
                return is_true;
            }

        }

    }

    return false;

}
