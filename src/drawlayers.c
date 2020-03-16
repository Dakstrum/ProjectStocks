#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_video.h>
#include <allegro5/allegro_primitives.h>

#include "log.h"
#include "audio.h"
#include "cache.h"
#include "drawlayers.h"

#include "draw.h"
#include "text.h"
#include "menu.h"
#include "graph.h"
#include "popup.h"
#include "video.h"
#include "button.h"
#include "textbox.h"
#include "scrollbox.h"
#include "jsonlayer.h"

#define MAX_DRAW_LAYERS 10
#define MAX_OBJECTS_PER_LAYER 256
#define MAX_MENU_WITH_CHILDS_PER_LAYER 64

typedef struct DrawLayer 
{

    DrawObject *objects[MAX_OBJECTS_PER_LAYER];
    MenuWithChilds *menu_with_childs[MAX_MENU_WITH_CHILDS_PER_LAYER];

} DrawLayer;

typedef struct ActiveTextBox
{

    DrawObject *object;

} ActiveTextBox;

static DrawLayer *draw_layers;
static int current_draw_layer = -1;

static ALLEGRO_DISPLAY *display = NULL;

static ActiveTextBox current_active_textbox;
static ALLEGRO_BITMAP *video_buffer;

void SetObjectPointersToNull();

void ClearUpDrawLayer(int layer);
void ClearUpGeneric(DrawObject *object);

bool HandleMouseClick(DrawObject *object, int x, int y);
bool IsMouseClickInAreaOfObject(DrawObject *object, int x, int y);

int AddDrawObjectToDrawLayer(DrawObject *object);

void DrawSingleLayer(DrawLayer *layer);
void DrawObjectOfTypeGen(DrawObject *object);
void DrawMenu(DrawObject *object);

void InitializeDrawLayers(ALLEGRO_DISPLAY *active_display) 
{

    al_destroy_bitmap(video_buffer);
    video_buffer = al_create_bitmap(1920, 1080);
    al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);

    display            = active_display;
    draw_layers        = malloc(sizeof(DrawLayer) * MAX_DRAW_LAYERS);
    current_draw_layer = -1;
    SetObjectPointersToNull();

}

int GetMaxObjectsPerDrawLayer() 
{

    return MAX_OBJECTS_PER_LAYER;

}

void SetObjectPointersToNull() 
{

    current_active_textbox.object = NULL;

    for (int i = 0; i < MAX_DRAW_LAYERS;i++) {

        for (int j = 0;j < MAX_OBJECTS_PER_LAYER; j++)
            draw_layers[i].objects[j] = NULL;

        for (int j = 0;j < MAX_MENU_WITH_CHILDS_PER_LAYER; j++)
            draw_layers[i].menu_with_childs[j] = NULL;

    }

}

void SetAllTextBoxesToInactiveInLayer(int layer)
{

    DrawObject **objects = draw_layers[layer].objects;
    for (int i = 0;i < MAX_OBJECTS_PER_LAYER;i++)
        if (objects[i] != NULL && objects[i]->type == TEXTBOX)
            objects[i]->bit_flags ^= (objects[i]->bit_flags & TEXTBOX_ACTIVE);

}

void SetAllTextBoxesToInactiveInCurrentDrawLayer()
{

    if (current_draw_layer == -1)
        return;

    SetAllTextBoxesToInactiveInLayer(current_draw_layer);

}

void SetActiveTextBox(DrawObject *object)
{
    
    if (current_active_textbox.object != NULL)
        current_active_textbox.object->bit_flags ^= (current_active_textbox.object->bit_flags & TEXTBOX_ACTIVE);

    current_active_textbox.object  = object;
    object->bit_flags             |= TEXTBOX_ACTIVE | TEXTBOX_FLICKER_DRAWING;
    object->textbox.flicker        = GetCurrentTime();

}

DrawObject** GetAllDrawObjectsInCurrentLayer() 
{

    if (current_draw_layer == -1)
        return NULL;

    return draw_layers[current_draw_layer].objects;

}

DrawObject *GetActiveTextBox()
{

    return current_active_textbox.object;

}

int CreateNewDrawLayer() 
{

    if (current_draw_layer + 1 == MAX_DRAW_LAYERS)
        return -1;

    SetAllTextBoxesToInactiveInCurrentDrawLayer();

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

void ClearUpMenuWithChilds(int layer)
{

    MenuWithChilds *menu = NULL;
    for (int j = 0;j < MAX_MENU_WITH_CHILDS_PER_LAYER;j++) {

        menu = draw_layers[layer].menu_with_childs[j];
        if (menu == NULL)
            continue;

        if (menu->text != NULL)
            free(menu->text);

        if (menu->buttons != NULL)
            free(menu->buttons);

        if (menu->text_boxes != NULL)
            free(menu->text_boxes);

        if (menu->scroll_boxes != NULL)
            free(menu->scroll_boxes);

        if (menu != NULL)
            free(menu);

        draw_layers[layer].menu_with_childs[j] = NULL;

    }

}

void ClearUpDrawObjects(int layer)
{

    for (int j = 0; j < MAX_OBJECTS_PER_LAYER; j++) {

        if (draw_layers[layer].objects[j] != NULL) {

            ClearUpGeneric(draw_layers[layer].objects[j]);
            free(draw_layers[layer].objects[j]);
            draw_layers[layer].objects[j] = NULL;

        }

    }

}

void ClearUpDrawLayer(int layer) 
{

    ClearUpDrawObjects(layer);
    ClearUpMenuWithChilds(layer);
    current_active_textbox.object = NULL;

}

void ClearUpGeneric(DrawObject *object) 
{

    object->name = NULL;
    switch (object->type) {

        case MENU     : CleanUpMenu(object);     break;
        case BUTTON   : CleanUpButton(object);   break;
        case POPUP    : CleanUpPopUp(object);    break;
        case VIDEO    : CleanUpVideo(object);    break;
        case TEXT     : CleanUpText(object);     break;
        case TEXTBOX  : CleanUpTextBox(object);  break;
        case SCROLLBOX: CleanUpScrollbox(object); break;

    }

}


/* SECTION: Add Objects to Current Draw Layer */
int AddButtonToDrawLayer(DrawObject *object) 
{

    InitButton(object);
    return AddDrawObjectToDrawLayer(object);

}

int AddTextBoxToDrawLayers(DrawObject *object)
{

    InitTextBox(object);
    return AddDrawObjectToDrawLayer(object);

}


int AddPopUpToDrawLayer(DrawObject *object)
{

    InitPopup(object, al_get_display_width(display), al_get_display_height(display));
    return AddDrawObjectToDrawLayer(object);

}

int AddGraphToDrawLayer(DrawObject *object) 
{

    return AddDrawObjectToDrawLayer(object);

}

int AddScrollBoxToDrawLayer(DrawObject *object) 
{

    if (object->asset_path == NULL) {

        Log("Unable to add scrollbox to draw layer.");
        return -1;

    }

    InitScrollbox(object);
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

    for (int i = 0; i < menu_with_childs->num_buttons; i++)
        AddObjectToDrawLayer(menu_with_childs->buttons[i]);

    for (int i = 0; i < menu_with_childs->num_text; i++)
        AddObjectToDrawLayer(menu_with_childs->text[i]);

    for (int i = 0; i < menu_with_childs->num_text_boxes; i++)
        AddObjectToDrawLayer(menu_with_childs->text_boxes[i]);

    for (int i = 0; i < menu_with_childs->num_scroll_boxes; i++)
        AddObjectToDrawLayer(menu_with_childs->scroll_boxes[i]);

    return 0;

}

int AddObjectToDrawLayer(DrawObject *object) 
{

    if (object == NULL)
        return -1;
    
    switch (object->type) {

        case MENU:      return AddMenuToDrawLayer(object);      break;
        case BUTTON:    return AddButtonToDrawLayer(object);    break;
        case POPUP:     return AddPopUpToDrawLayer(object);     break;
        case VIDEO:     return AddVideoToDrawLayer(object);     break;
        case TEXT:      return AddTextToDrawLayer(object);      break;
        case TEXTBOX:   return AddTextBoxToDrawLayers(object);  break;
        case GRAPH:     return AddGraphToDrawLayer(object);     break;
        case SCROLLBOX: return AddScrollBoxToDrawLayer(object); break;

    }

    return -1;

}

int AddMenuToDrawLayer(DrawObject *object) 
{

    InitMenu(object);
    if (object->menu.menu_bitmap == NULL) {

        Log("Could not create menu_bitmap");
        return -1;

    }
    return AddDrawObjectToDrawLayer(object);

}

int AddVideoToDrawLayer(DrawObject *object) 
{

    InitVideo(object);
    if (object->video.video == NULL) {

        LogF("Unable to open %s", object->asset_path);
        return -1;

    }
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

    InitText(object);
    if (object->text.font == NULL) {

        LogF("Unable to open font %s", object->asset_path);
        return -1;

    }

    return AddDrawObjectToDrawLayer(object);

}


/* SECTION: Drawing functions */
void DrawLayers() 
{

    RefreshDrawScale();
    al_set_target_bitmap(video_buffer);

    for (int i = 0; i < current_draw_layer+1; i++)
        DrawSingleLayer(&draw_layers[i]);

    al_set_target_backbuffer(display);
    DrawBackBuffer(video_buffer);

}

void DrawSingleLayer(DrawLayer *layer) 
{

    for (int i = 0; i < MAX_OBJECTS_PER_LAYER; i++) 
        if (layer->objects[i] != NULL && layer->objects[i]->bit_flags & SHOULD_BE_DRAWN)
            DrawObjectOfTypeGen(layer->objects[i]);

}

void CheckToRemovePopup(DrawObject *object)
{

    if (object->bit_flags & CAN_BE_REMOVED)
        RemoveDrawObject(object);

}

void DrawObjectOfTypeGen(DrawObject *object) 
{

    switch (object->type) {

        case MENU:      DrawMenu(object);      break;
        case BUTTON:    DrawButton(object);    break;
        case POPUP:     DrawPopUp(object); CheckToRemovePopup(object); break;
        case VIDEO:     DrawVideo(object);     break;
        case TEXT:      DrawText(object);      break;
        case TEXTBOX:   DrawTextBox(object);   break;
        case GRAPH:     DrawGraph(object);     break;
        case SCROLLBOX: DrawScrollBox(object); break;

    }

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

    if (object->layer_index > current_draw_layer) {

        Log("Attempt to remove object that does not exist");
        return -1;

    }

    if (draw_layers[object->layer_index].objects[object->object_index] == NULL) {

        LogF("object with name %s", object->name);
        return -1;

    }
    ClearUpGeneric(object);

    const int layer_index  = object->layer_index;
    const int object_index = object->object_index;
    free(draw_layers[layer_index].objects[object_index]);
    draw_layers[layer_index].objects[object_index] = NULL;
    
    return 0;

}

DrawObject *FindDrawObject(char *object_name)
{

    if (current_draw_layer == -1)
        return NULL;

    DrawObject *object = NULL;
    for (int i = 0; i < MAX_OBJECTS_PER_LAYER; i++) {

        object = draw_layers[current_draw_layer].objects[i];
        if (object != NULL && object->name != NULL)
            if (strcmp(object->name, object_name) == 0)
                return object;

    }

    return NULL;

}

bool DoesObjectExistInCurrentDrawLayer(char *object_name)
{
 
    return FindDrawObject(object_name) != NULL ? true : false;

}

char *GetTextFromTextBox(char *object_name) 
{

    DrawObject *object = FindDrawObject(object_name);
    if (object == NULL)
        return NULL;

    return object->textbox.text;

}


DrawObjectTypeCollection *GetObjectsByType(DrawType type)
{

    if (current_draw_layer == -1)
        return NULL;

    DrawObjectTypeCollection *collection = malloc(sizeof(DrawObjectTypeCollection));
    collection->num_objects = 0;
    collection->objects     = malloc(sizeof(DrawObject *) * MAX_OBJECTS_PER_LAYER);
    DrawObject *object      = NULL;
    for (int i = 0; i < MAX_OBJECTS_PER_LAYER; i++) {

        object = draw_layers[current_draw_layer].objects[i];
        if (object != NULL && object->type == type) {

            collection->objects[collection->num_objects] = object;
            collection->num_objects++;

        }

    }
    collection->objects = realloc(collection->objects, sizeof(DrawObject *) * collection->num_objects + 1);
    return collection;

}

DrawObject *GetObjectAndDraw(char* object_name)
{

    DrawObject *object = GetDrawObjectFromJsonLayer(object_name);
    AddObjectToDrawLayer(object);
    return object;

}

void DisposeDrawObjectTypeCollection(DrawObjectTypeCollection *collection)
{

    if (collection == NULL)
        return;

    if (collection->objects != NULL)
        free(collection->objects);

    free(collection);

}