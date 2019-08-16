
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "drawlayers.h"

typedef struct DrawLayer 
{

    unsigned int num_objects;
    DrawObject objects[256];

} DrawLayer;


static DrawLayer *draw_layers;
static unsigned int num_draw_layers = 0;

void CleanUpLayer(int layer);
void CleanUpButton(DrawObject *object);
void CleanUpMenu(DrawObject *object);
void CleanUpPopUp(DrawObject *object);
void CleanUpGeneric(DrawObject *object);

bool HandleMouseClick(DrawObject *object, int x, int y);
bool IsMouseClickInAreaOfObject(DrawObject *object, int x, int y);

void InitializeDrawLayers() 
{

    draw_layers = malloc(sizeof(DrawLayer) * 10);

}

void HandleMouseClickInButtonAreas(int x, int y) 
{

    for (int i = 0; i < draw_layers[num_draw_layers - 1].num_objects;i++)
        if (draw_layers[num_draw_layers - 1].objects[i].type == BUTTON)
            HandleMouseClick(&draw_layers[num_draw_layers - 1].objects[i], x, y);

}

bool HandleMouseClick(DrawObject *object, int x, int y) 
{

    if (IsMouseClickInAreaOfObject(object, x, y)
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

void CleanCurrentLayer() 
{

    CleanUpLayer(num_draw_layers - 1);

}

void CleanUpLayer(int layer) 
{

    for (int j = 0; j < draw_layers[layer].num_objects; j++)
        CleanUpGeneric(&draw_layers[layer].objects[j]);

}

void CleanUpGeneric(DrawObject *object) 
{

    switch (object->type) {

        case MENU:   CleanUpMenu(object);   break;
        case BUTTON: CleanUpButton(object); break;
        case POPUP:  CleanUpPopUp(object);  break;

    }

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

void ClearLayers() 
{

    for (int i = 0; i < num_draw_layers; i++)
        CleanUpLayer(i);

    free(draw_layers);
    InitializeDrawLayers();

}