
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_video.h>

#include "log.h"
#include "audio.h"
#include "drawlayers.h"

#define MAX_DRAW_LAYERS 10

typedef struct DrawLayer 
{

    unsigned int num_objects;
    DrawObject *objects[256];

} DrawLayer;


static DrawLayer *draw_layers;
static unsigned int current_draw_layer = -1;

static ALLEGRO_DISPLAY *display     = NULL;

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
void DrawGeneric(ALLEGRO_BITMAP *bitmap, float x, float y);
void DrawGenericWithWidth(ALLEGRO_BITMAP *bitmap, float x, float y, float width, float height);

void InitializeDrawLayers(ALLEGRO_DISPLAY *active_display) 
{

    display     = active_display;
    draw_layers = malloc(sizeof(DrawLayer) * MAX_DRAW_LAYERS);
    for (int i = 0; i < MAX_DRAW_LAYERS; i++) {

        draw_layers[i].num_objects = 0;

    }
    current_draw_layer = -1;

}

void HandleMouseClickInButtonAreas(int x, int y) 
{

    for (int i = 0; i < draw_layers[current_draw_layer].num_objects;i++)
        if (draw_layers[current_draw_layer].objects[i]->type == BUTTON)
            HandleMouseClick(draw_layers[current_draw_layer].objects[i], x, y);

}

bool HandleMouseClick(DrawObject *object, int x, int y) 
{

    if (IsMouseClickInAreaOfObject(object, x, y))
        object->member.button.Callback();

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

    ClearUpDrawLayer(current_draw_layer - 1);
    current_draw_layer--;

}

void ClearUpDrawLayer(int layer) 
{

    for (int j = 0; j < draw_layers[layer].num_objects; j++) {

        ClearUpGeneric(draw_layers[layer].objects[j]);
        free(draw_layers[layer].objects[j]);

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

void ClearDrawLayers() 
{

    for (int i = 0; i < current_draw_layer; i++)
        ClearUpDrawLayer(i);

    free(draw_layers);
    InitializeDrawLayers(display);

}

void CleanUpButton(DrawObject *object) 
{

    al_destroy_bitmap(object->member.button.button_bitmap);

}

void CleanUpMenu(DrawObject *object) 
{

    for (int i = 0; i < object->member.menu.num_buttons; i++)
        al_destroy_bitmap(object->member.menu.buttons[i].button_bitmap);

    free(object->member.menu.buttons);

}

void CleanUpPopUp(DrawObject *object) 
{

    Log("PopUp STUB");

}

void CleanUpVideo(DrawObject *object) 
{

    al_close_video(object->member.video.video);

}


/* SECTION: Add Objects to Current Draw Layer */
int AddButtonToDrawLayer(DrawObject *object) 
{

    return -1;

}

int AddMenuToDrawLayer(DrawObject *object) 
{

    object->member.menu.menu_bitmap = al_load_bitmap(object->member.menu.picture_path);
    if (object->member.menu.menu_bitmap == NULL) {

        Log("Could not create menu_bitmap");
        return -1;

    }

    return AddDrawObjectToDrawLayer(object);

}

int AddVideoToDrawLayer(DrawObject *object, bool start_video_immediately) 
{

    object->member.video.video = al_open_video(object->member.video.video_path);
    if (object->member.video.video == NULL) {

        LogF("Unable to open %s", object->member.video.video_path);
        al_rest(1);
        return -1;
    }
    if (start_video_immediately)
        al_start_video(object->member.video.video, al_get_default_mixer());

    return AddDrawObjectToDrawLayer(object);

}

int AddDrawObjectToDrawLayer(DrawObject *object) 
{

    DrawLayer *layer = &draw_layers[current_draw_layer];
    layer->objects[layer->num_objects] = object;
    layer->num_objects++;
    return layer->num_objects - 1;

}



/* SECTION: Drawing functions */
void DrawLayers() 
{

    for (int i = 0; i < current_draw_layer+1;i++)
        DrawSingleLayer(&draw_layers[i]);
}

void DrawSingleLayer(DrawLayer *layer) 
{

    for (int i = 0; i < layer->num_objects; i++) {
        if (layer->objects[i]->should_this_be_drawn)
            DrawObjectOfTypeGen(layer, i);
    }


}

void DrawObjectOfTypeGen(DrawLayer *layer, int i) 
{

    switch (layer->objects[i]->type) {

        case MENU:   DrawMenu(layer->objects[i]);   break;
        case BUTTON: DrawButton(layer->objects[i]); break;
        case POPUP:   break;
        case VIDEO: DrawVideo(layer->objects[i]);   break;

    }

}

void DrawMenu(DrawObject *object) 
{

    if (object->scale_to_entire_screen)
        DrawGenericWithWidth(object->member.menu.menu_bitmap, object->x, object->y, al_get_display_width(display), al_get_display_height(display));
    else if (object->width != 0.0f && object->height != 0.0f)
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

    if (object->scale_to_entire_screen)
        DrawGenericWithWidth(al_get_video_frame(object->member.video.video), object->x, object->y, al_get_display_width(display), al_get_display_height(display));

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
    al_draw_scaled_bitmap(bitmap, 0, 0, scale_width, scale_height, x, y, width, height, 0);

}

DrawObject *GetDrawObject(int layer, int object) 
{

    return draw_layers[layer].objects[object];

}

DrawObject *CreateNewDrawObject() 
{

    return malloc(sizeof(DrawObject));

}