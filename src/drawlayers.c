#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_video.h>
#include<allegro5/allegro_primitives.h>

#include "log.h"
#include "audio.h"
#include "cache.h"
#include "drawlayers.h"

#define MAX_DRAW_LAYERS 10
#define MAX_OBJECTS_PER_LAYER 256
#define MAX_MENU_WITH_CHILDS_PER_LAYER 64

typedef struct DrawLayer 
{

    DrawObject *objects[MAX_OBJECTS_PER_LAYER];
    MenuWithChilds *menu_with_childs[MAX_MENU_WITH_CHILDS_PER_LAYER];

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
void CleanUpText(DrawObject *object);
void ClearUpGeneric(DrawObject *object);

bool HandleMouseClick(DrawObject *object, int x, int y);
bool IsMouseClickInAreaOfObject(DrawObject *object, int x, int y);

int AddDrawObjectToDrawLayer(DrawObject *object);

void DrawSingleLayer(DrawLayer *layer);
void DrawObjectOfTypeGen(DrawLayer *layer, int i);
void DrawMenu(DrawObject *object);
void DrawButton(DrawObject *object);
void DrawPopUp(DrawObject *object);
void DrawVideo(DrawObject *object);
void DrawText(DrawObject *object);
void DrawGraph(DrawObject *object);
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

    for (int i = 0; i < MAX_DRAW_LAYERS;i++) {

        for (int j = 0;j < MAX_OBJECTS_PER_LAYER; j++)
            draw_layers[i].objects[j] = NULL;

        for (int j = 0;j < MAX_MENU_WITH_CHILDS_PER_LAYER; j++)
            draw_layers[i].menu_with_childs[j] = NULL;

    }

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

        if (object->button.Callback != NULL) {

            object->button.Callback();
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

    for (int j = 0;j < MAX_MENU_WITH_CHILDS_PER_LAYER;j++) {

        if (draw_layers[layer].menu_with_childs[j] != NULL) {

            free(draw_layers[layer].menu_with_childs[j]->buttons);
            free(draw_layers[layer].menu_with_childs[j]->text);
            free(draw_layers[layer].menu_with_childs[j]);
            draw_layers[layer].menu_with_childs[j] = NULL;

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
        case TEXT:   CleanUpText(object);   break;

    }

}

void CleanUpButton(DrawObject *object) 
{

    //al_destroy_bitmap(object->button.button_bitmap);

}

void CleanUpMenu(DrawObject *object) 
{

    //al_destroy_bitmap(object->menu.menu_bitmap);

}

void CleanUpPopUp(DrawObject *object) 
{
    

}

void CleanUpVideo(DrawObject *object) 
{

    al_close_video(object->video.video);

}

void CleanUpText(DrawObject *object) 
{

    //al_destroy_font(object->text.font);

}


/* SECTION: Add Objects to Current Draw Layer */
int AddButtonToDrawLayer(DrawObject *object) 
{

    if (object->asset_path != NULL)
        object->button.button_bitmap = GetBitmapFromCache(object->asset_path);

    return AddDrawObjectToDrawLayer(object);

}



int AddPopUpToDrawLayer(DrawObject *object)
{

    if (object->asset_path != NULL)
        object->popup.popup_bitmap = GetBitmapFromCache(object->asset_path);

    return AddDrawObjectToDrawLayer(object);


}

int AddGraphToDrawLayer(DrawObject *object) 
{

    return AddDrawObjectToDrawLayer(object);

}

void StoreMenuWithChildsRefOnDrawLayer(MenuWithChilds *menu_with_childs) 
{

    menu_with_childs->draw_layer = current_draw_layer;
    for (int i = 0; i < MAX_MENU_WITH_CHILDS_PER_LAYER;i++) {

        if (draw_layers[current_draw_layer].menu_with_childs[i] == NULL) {

            draw_layers[current_draw_layer].menu_with_childs[i] = menu_with_childs;
            break;

        }

    }
}

int AddMenuWithChildsToDrawLayer(MenuWithChilds *menu_with_childs) 
{

    StoreMenuWithChildsRefOnDrawLayer(menu_with_childs);
    AddObjectToDrawLayer(menu_with_childs->menu);
    for (int i = 0; i < menu_with_childs->num_buttons;i++)
        AddObjectToDrawLayer(menu_with_childs->buttons[i]);
    for (int i = 0; i < menu_with_childs->num_text;i++)
        AddObjectToDrawLayer(menu_with_childs->text[i]);

    return 0;

}

int AddObjectToDrawLayer(DrawObject *object) 
{

    switch (object->type) {

        case MENU:   return AddMenuToDrawLayer(object);   break;
        case BUTTON: return AddButtonToDrawLayer(object); break;
        case POPUP:  return AddPopUpToDrawLayer(object);  break;
        case VIDEO:  return AddVideoToDrawLayer(object);  break;
        case TEXT:   return AddTextToDrawLayer(object);   break;
        case GRAPH:  return AddGraphToDrawLayer(object);  break;

    }

    return -1;

}

int AddMenuToDrawLayer(DrawObject *object) 
{

    object->menu.menu_bitmap = GetBitmapFromCache(object->asset_path);
    if (object->menu.menu_bitmap == NULL) {

        Log("Could not create menu_bitmap");
        return -1;

    }

    return AddDrawObjectToDrawLayer(object);

}

int AddVideoToDrawLayer(DrawObject *object) 
{

    object->video.video = al_open_video(object->asset_path);
    if (object->video.video == NULL) {

        LogF("Unable to open %s", object->asset_path);
        return -1;
    }
    LogF("Adding Video %s to DrawLayer", object->asset_path);
    if (object->video.start_video_immediately)
        al_start_video(object->video.video, al_get_default_mixer());

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

    object->text.font  = GetFontFromCache(object->asset_path, object->text.font_size);
    object->text.color = al_map_rgba(object->text.r, object->text.g, object->text.b, object->text.a);

    if (object->text.font == NULL) {

        LogF("Unable to open font %s", object->asset_path);
        return -1;

    }

    return AddDrawObjectToDrawLayer(object);

}


/* SECTION: Drawing functions */
void DrawLayers() 
{

    for (int i = 0; i < current_draw_layer+1; i++)
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
        case POPUP:  DrawPopUp(layer->objects[i]);  break;
        case VIDEO:  DrawVideo(layer->objects[i]);  break;
        case TEXT:   DrawText(layer->objects[i]);   break;
        case GRAPH:  DrawGraph(layer->objects[i]);   break;

    }

}

void DrawGraph(DrawObject *object) 
{

    float x = object->x;
    float y = object->y;
    float y_start_point = object->y + object->height;
    Point *points = object->graph.points;
    ALLEGRO_COLOR color = al_map_rgba(255, 255, 255, 255);
    for (unsigned int i = 0;i < object->graph.num_points - 1;i++) {

        al_draw_line(x + points[i].x, y_start_point - points[i].y, x + points[i+1].x, y_start_point - points[i+1].y, color ,1);

    }
    
}

void DrawPopUp(DrawObject *object)
{

    if(object->popup.current_time >= object->popup.end_time) {

        RemoveDrawObject(object);

    } else {
        
        DrawGenericWithWidth(object->popup.popup_bitmap, object->x, object->y, object->width, object->height);
        object->popup.current_time++;

    }

}

void DrawMenu(DrawObject *object) 
{

    if (object->width != 0.0f && object->height != 0.0f)
        DrawGenericWithWidth(object->menu.menu_bitmap, object->x, object->y, object->width, object->height);
    else
        DrawGeneric(object->menu.menu_bitmap, object->x, object->y);

}

void DrawButton(DrawObject *object) 
{

    DrawGeneric(object->button.button_bitmap, object->x, object->y);

}

void DrawVideo(DrawObject *object) 
{

    if (!al_is_video_playing(object->video.video))
        return;

    DrawGenericWithWidth(al_get_video_frame(object->video.video), object->x, object->y, object->width, object->height);

}

void DrawText(DrawObject *object) 
{

    al_draw_text(object->text.font, object->text.color, object->x, object->y, 0, object->text.content);

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
