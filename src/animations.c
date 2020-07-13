
#include "drawobject.h"
#include "vector.h"
#include "shared.h"

#include <time.h>

typedef struct MoveAnimation
{

    unsigned char layer_index;
    struct timespec stop_time;

    float *x;
    float *y;

    float dy;
    float dx;

} MoveAnimation;

Vector *move_objects = NULL;

void Animate_Initialize()
{

    move_objects = Vector_Create(sizeof(MoveAnimation), 8);

}

void Animate_MoveDrawObject(DrawObject *object, float n_x, float n_y, long animation_length)
{

    MoveAnimation anim;
    anim.layer_index = object->layer_index;
    anim.x           = &object->x;
    anim.y           = &object->y;
    anim.dx          = (n_x - object->x)/animation_length;
    anim.dy          = (n_y - object->y)/animation_length;
    anim.stop_time   = GetOffsetTime(animation_length);

    Vector_PushBack(move_objects, &anim);

}


void Animate_MoveDrawObjects()
{

    MoveAnimation *anims = move_objects->elements;
    for (size_t i = 0; i < move_objects->num_elements; i++) {

        *anims[i].x += anims[i].dx;
        *anims[i].y += anims[i].dy;

    }

}

void Animate_DisableMoveDrawObjects()
{



}


void Animate_Update()
{

    struct timespec current_time = GetCurrentTime();

    Animate_MoveDrawObjects();
    Animate_DisableMoveDrawObjects();

}

void Animate_DisableByLayer(unsigned char layer_index)
{



}