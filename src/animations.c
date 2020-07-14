
#include "drawobject.h"
#include "vector.h"
#include "shared.h"
#include "log.h"

#include <time.h>

typedef struct Animation {

    unsigned char layer_index;

} Animation;

typedef struct MoveAnimation
{

    unsigned char layer_index;
    struct timespec stop_time;

    float *x;
    float *y;

    float v_x;
    float v_y;

} MoveAnimation;

Vector *move_objects = NULL;
struct timespec last_animation_update;

void Animate_Initialize()
{

    move_objects = Vector_Create(sizeof(MoveAnimation), 8);
    last_animation_update.tv_sec  = 0;
    last_animation_update.tv_nsec = 0;

}

void Animate_MoveDrawObject(DrawObject *object, float n_x, float n_y, long animation_length)
{

    MoveAnimation anim;
    anim.layer_index = object->layer_index;
    anim.x           = &object->x;
    anim.y           = &object->y;
    anim.v_x         = (n_x - object->x)/animation_length;
    anim.v_y         = (n_y - object->y)/animation_length;
    anim.stop_time   = GetOffsetTime(animation_length);

    Vector_PushBack(move_objects, &anim);

}


void Animate_MoveDrawObjects(long milli_diff)
{

    MoveAnimation *anims = move_objects->elements;
    for (size_t i = 0; i < move_objects->num_elements; i++) {

        *anims[i].x += anims[i].v_x * milli_diff;
        *anims[i].y += anims[i].v_y * milli_diff;

    }

}

void Animate_DisableMoveDrawObjects()
{

    MoveAnimation *anims = move_objects->elements;
    for (size_t i = 0; i < move_objects->num_elements;i++) {

        if (IsTimeSpecInPast(&anims->stop_time)) {

            Vector_Remove(move_objects, i);
            i--;

        }

    }

}


void Animate_Update()
{

    struct timespec current_time = GetCurrentTime();
    long milli_diff = GetMilliDiff(&current_time, &last_animation_update);

    if (milli_diff == 0)
        return;

    last_animation_update = current_time;
    Animate_MoveDrawObjects(milli_diff);
    Animate_DisableMoveDrawObjects();

}

void Animate_DisableGenByLayer(unsigned char layer_index, Vector *vec)
{

    const unsigned int size_of_element = vec->size_of_single_elem;
    Animation *animations = vec->elements;
    for (size_t i = 0; i < vec->num_elements; i++) {

        if (animations[i*size_of_element].layer_index == layer_index) {

            Vector_Remove(vec, i);
            i--;

        }

    }

}

void Animate_DisableByLayer(unsigned char layer_index)
{

    Animate_DisableGenByLayer(layer_index, move_objects);

}