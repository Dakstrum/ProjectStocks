
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_video.h>

#include "log.h"
#include "drawlayers.h"

#define MAX_DRAW_LAYERS 10

typedef struct DrawLayer 
{

    unsigned int num_objects;
    DrawObject *objects[256];

} DrawLayer;


static DrawLayer *draw_layers;
static unsigned int num_draw_layers = -1;

static ALLEGRO_DISPLAY *display     = NULL;

void ClearUpDrawLayer(int layer);
void CleanUpButton(DrawObject *object);
void CleanUpMenu(DrawObject *object);
void CleanUpPopUp(DrawObject *object);
void ClearUpGeneric(DrawObject *object);

bool HandleMouseClick(DrawObject *object, int x, int y);
bool IsMouseClickInAreaOfObject(DrawObject *object, int x, int y);

int AddDrawObjectToDrawLayer(DrawObject *object);

void DrawSingleLayer(DrawLayer *layer);
void DrawObjectOfTypeGen(DrawLayer *layer, int i);
void DrawMenu(Menu *menu, bool fill_screen);
void DrawButton(Button *button);
void DrawGeneric(ALLEGRO_BITMAP *bitmap, float x, float y);
void DrawGenericWithWidth(ALLEGRO_BITMAP *bitmap, float x, float y, float width, float height);

void InitializeDrawLayers(ALLEGRO_DISPLAY *active_display) 
{

    display     = active_display;
    draw_layers = malloc(sizeof(DrawLayer) * MAX_DRAW_LAYERS);
    for (int i = 0; i < MAX_DRAW_LAYERS; i++) {

        draw_layers[i].num_objects = 0;

    }

}

void HandleMouseClickInButtonAreas(int x, int y) 
{

    for (int i = 0; i < draw_layers[num_draw_layers].num_objects;i++)
        if (draw_layers[num_draw_layers].objects[i]->type == BUTTON)
            HandleMouseClick(draw_layers[num_draw_layers].objects[i], x, y);

}

bool HandleMouseClick(DrawObject *object, int x, int y) 
{

    if (IsMouseClickInAreaOfObject(object, x, y))
        object->member.button.Callback();

}

bool IsMouseClickInAreaOfObject(DrawObject *object, int x, int y) 
{

    Button button = object->member.button;
    if (button.x > x || x > button.x + button.width)
        return false;
    if (button.y > y || y > button.y + button.height)
        return false;

    return true;

}

/* SECTION: Draw Layer Creation and Cleanup */
int CreateNewDrawLayer() 
{

    if (num_draw_layers + 1 == MAX_DRAW_LAYERS) {

        return -1;

    }
    num_draw_layers++;
    return num_draw_layers;

}

void ClearCurrentDrawLayer() 
{

    ClearUpDrawLayer(num_draw_layers - 1);
    num_draw_layers--;

}

void ClearUpDrawLayer(int layer) 
{

    for (int j = 0; j < draw_layers[layer].num_objects; j++)
        ClearUpGeneric(draw_layers[layer].objects[j]);

}

void ClearUpGeneric(DrawObject *object) 
{

    switch (object->type) {

        case MENU:   CleanUpMenu(object);   break;
        case BUTTON: CleanUpButton(object); break;
        case POPUP:  CleanUpPopUp(object);  break;

    }

}

void ClearDrawLayers() 
{

    for (int i = 0; i < num_draw_layers; i++)
        ClearUpDrawLayer(i);

    free(draw_layers);
    InitializeDrawLayers(display);

}

void CleanUpButton(DrawObject *object) 
{

    free(object->member.button.picture_path);

}

void CleanUpMenu(DrawObject *object) 
{

    for (int i = 0; i < object->member.menu.num_buttons; i++)
        free(object->member.menu.buttons[i].picture_path);

    free(object->member.menu.buttons);

}

void CleanUpPopUp(DrawObject *object) 
{

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

int AddVideoToDrawLayer(DrawObject *object) 
{

    object->member.video.video = al_open_video(object->member.video.video_path);
    return AddDrawObjectToDrawLayer(object);

}

int AddDrawObjectToDrawLayer(DrawObject *object) 
{

    DrawLayer *layer = &draw_layers[num_draw_layers];
    layer->objects[layer->num_objects] = object;
    layer->num_objects++;
    return layer->num_objects - 1;

}



/* SECTION: Drawing functions */
void DrawLayers() 
{

    for (int i = 0; i < num_draw_layers+1;i++)
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

        case MENU:   DrawMenu(&layer->objects[i]->member.menu, layer->objects[i]->scale_to_entire_screen);     break;
        case BUTTON: DrawButton(&layer->objects[i]->member.button); break;
        case POPUP:   break;

    }

}

void DrawMenu(Menu *menu, bool fill_screen) 
{

    if (fill_screen)
        DrawGenericWithWidth(menu->menu_bitmap, menu->x, menu->y, al_get_display_width(display), al_get_display_height(display));
    else if (menu->width != 0.0f && menu->height != 0.0f)
        DrawGenericWithWidth(menu->menu_bitmap, menu->x, menu->y, menu->width, menu->height);
    else
        DrawGeneric(menu->menu_bitmap, menu->x, menu->y);

}

void DrawButton(Button *button) 
{

    if (button->button_bitmap == NULL)
        return;

    DrawGeneric(button->button_bitmap, button->x, button->y);

}

void DrawVideo(Video *video, bool fill_screen) 
{

}

void DrawGeneric(ALLEGRO_BITMAP *bitmap, float x, float y) 
{

    float scale_width  = al_get_bitmap_width(bitmap);
    float scale_height = al_get_bitmap_height(bitmap);
    al_draw_scaled_bitmap(bitmap, 0, 0, scale_width, scale_height, x, y, scale_width, scale_height, 0);

}

void DrawGenericWithWidth(ALLEGRO_BITMAP *bitmap, float x, float y, float width, float height) 
{

    float scale_width  = al_get_bitmap_width(bitmap);
    float scale_height = al_get_bitmap_height(bitmap);
    al_draw_scaled_bitmap(bitmap, 0, 0, scale_width, scale_height, x, y, width, height, 0);

}

DrawObject *GetDrawObject(int layer, int object) 
{

    return draw_layers[layer].objects[object];

}

DrawObject *GetNewDrawObject() 
{

    return malloc(sizeof(DrawObject));

}