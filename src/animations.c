
#include <time.h>

#include <allegro5/allegro.h>

#include "drawobject.h"
#include "vector.h"
#include "shared.h"
#include "log.h"
#include "physics.h"

typedef struct Animation {

    unsigned char layer_index;

} Animation;

typedef struct MoveAnimation
{

    unsigned char layer_index;
    float animation_length;

    float *x;
    float *y;

    float n_x;
    float n_y;

} MoveAnimation;

Vector *move_objects = NULL;

void Animate_Initialize()
{

    move_objects  = Vector_Create(sizeof(MoveAnimation), 8);

}

void Animate_MoveDrawObject(DrawObject *object, float n_x, float n_y, long animation_length)
{

    MoveAnimation anim;
    anim.layer_index = object->layer_index;
    anim.x           = &object->x;
    anim.y           = &object->y;
    anim.n_x         = n_x;
    anim.n_y         = n_y;
    anim.animation_length = (float)animation_length;

    Vector_PushBack(move_objects, &anim);

}


void Animate_MoveDrawObjects(double milli_diff)
{

    MoveAnimation *anims = move_objects->elements;
    for (size_t i = 0; i < move_objects->num_elements; i++) {

        *anims[i].x += ((anims[i].n_x - *anims[i].x) / anims[i].animation_length) * milli_diff;
        *anims[i].y += ((anims[i].n_y - *anims[i].y) / anims[i].animation_length) * milli_diff;
        anims[i].animation_length -= milli_diff;

    }

}

void Animate_DisableMoveDrawObjects()
{

    MoveAnimation *anims = move_objects->elements;
    for (size_t i = 0; i < move_objects->num_elements;i++) {

        if (anims[i].animation_length <= 0) {

            Vector_Remove(move_objects, i);
            i--;

        }

    }

}


void Animate_Update(struct timespec last_update_time, struct timespec new_update_time)
{

    double milli_diff = GetDoubleMilliDiff(&new_update_time, &last_update_time);

    LogF("milli_diff = %f", milli_diff);
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