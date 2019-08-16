
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef enum DrawType 
{

    MENU = 0,
    BUTTON,
    POPUP

} DrawType;


typedef struct DrawObject 
{

    DrawType type;
    float x;
    float y;
    float width;
    float height;
    union {

        struct {

            char picture_path[256];

        } menu;

        struct {

            void (*Callback)();

        } button;

    } member;

} DrawObject;

typedef struct DrawLayer 
{

    unsigned int num_objects;
    DrawObject objects[256];

} DrawLayer;

static DrawLayer *draw_layers;
static unsigned int current_draw_layer = 0;


bool HandleMouseClick(DrawObject *object, int x, int y);
bool IsMouseClickInAreaOfObject(DrawObject *object, int x, int y);

void InitializeDrawLayers() 
{

    draw_layers = malloc(sizeof(DrawLayer) * 10);

}

void HandleMouseClickInButtonAreas(int x, int y) 
{

    for (int i = 0; i < draw_layers[current_draw_layer].num_objects;i++) {

        if (draw_layers[current_draw_layer].objects[i].type == BUTTON) {

            HandleMouseClick(&draw_layers[current_draw_layer].objects[i], x, y);

        }

    }

}

bool HandleMouseClick(DrawObject *object, int x, int y) 
{

    if (IsMouseClickInAreaOfObject(object, x, y)) {

        object->member.button.Callback();

    }

}

bool IsMouseClickInAreaOfObject(DrawObject *object, int x, int y) 
{

    if (object->x > x || x > object->x + object->width)
        return false;
    if (object->y > y || y > object->y + object->height)
        return false;

    return true;

}

void CleanUpLayers() 
{

    free(draw_layers);

}

void ClearLayers() 
{

    CleanUpLayers();
    InitializeDrawLayers();

}