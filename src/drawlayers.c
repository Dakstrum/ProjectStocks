
#include <stdio.h>
#include <stdlib.h>


typedef enum DrawType 
{

    MENU = 0,
    BUTTON,
    POPUP

} DrawType;


typedef struct DrawObject 
{

    DrawType type;
    float x_pos;
    float y_pos;
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

void InitializeDrawLayers() 
{

    draw_layers = malloc(sizeof(DrawLayer) * 10);

}

void HandleMouseClickInButtonAreas(int x, int y) 
{

    for (int i = 0; i < draw_layers[current_draw_layer].num_objects;i++) {


        DrawObject *object = &draw_layers[current_draw_layer].objects[i];
        if (object->type == BUTTON) {

            object->member.button.Callback();

        }

    }

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