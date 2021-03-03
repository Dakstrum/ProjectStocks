
#include <time.h>
#include <assert.h>

#include <allegro5/allegro.h>

#include "drawobject.h"
#include "vector.h"
#include "shared.h"
#include "log.h"
#include "physics.h"

typedef struct Animation {

    unsigned char layer_index;
    unsigned int id;
    unsigned int group_id;

} Animation;

typedef struct MoveAnimation
{

    unsigned char layer_index;
    unsigned int id;
    unsigned int group_id;
    float animation_length;

    float *x;
    float *y;

    float n_x;
    float n_y;

} MoveAnimation;

Vector *move_objects                 = NULL;
static unsigned int id_counter       = 1;
static unsigned int group_id_counter = 1;
static bool group_id_active          = false;

void Animate_Initialize()
{

    move_objects = Vector_Create(sizeof(MoveAnimation), 8);

}

unsigned int Animation_GetId() 
{

    id_counter++;
    return id_counter - 1;

}

unsigned int Animation_GetGroupId()
{

    if (!group_id_active)
        return 0;

    return group_id_counter;


}

unsigned int Animation_StartGroup() 
{

    group_id_active = true;
    return group_id_counter;

}

void Animation_EndGroup()
{

    group_id_counter++;
    group_id_active = false;

}

unsigned int Animate_MoveDrawObject(DrawObject *object, float n_x, float n_y, long animation_length)
{

    MoveAnimation anim;
    anim.layer_index = object->layer_index;
    anim.x           = &object->x;
    anim.y           = &object->y;
    anim.id          = Animation_GetId();
    anim.group_id    = Animation_GetGroupId();
    anim.n_x         = n_x;
    anim.n_y         = n_y;
    anim.animation_length = (float)animation_length;

    Vector_PushBack(move_objects, &anim);

    return anim.id;

}


void Animate_MoveDrawObjects(double milli_diff)
{

    double dt = 0;
    MoveAnimation *anims = move_objects->elements;
    for (size_t i = 0; i < move_objects->num_elements; i++) {

        dt = milli_diff;
        if (anims[i].animation_length < milli_diff)
            dt = anims[i].animation_length;

        if (anims[i].animation_length == 0 || dt == 0)
            continue;

        if (anims[i].n_x != *anims[i].x)
            *anims[i].x += ((anims[i].n_x - *anims[i].x) / anims[i].animation_length) * dt;

        if (anims[i].n_y != *anims[i].y) 
            *anims[i].y += ((anims[i].n_y - *anims[i].y) / anims[i].animation_length) * dt;

        anims[i].animation_length -= dt;

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


void Animate_Update(double dt)
{

    Animate_DisableMoveDrawObjects();
    Animate_MoveDrawObjects(dt);
    
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

bool Animate_FinishedGenGroupAnimation(unsigned int group_id, Vector *vec)
{

    Vector_ForEach(i, anim, vec, void *) {

        Animation *temp = anim;
        if (temp->group_id == group_id)
            return false;


    }

    return true;

}

bool Animate_FinishedGenAnimation(unsigned int id, Vector *vec)
{

    Vector_ForEach(i, anim, vec, void *) {

        Animation *temp = anim;
        if (temp->id == id)
            return false;


    }

    return true;

}

bool Animate_FinishedMoveAnimation(unsigned int id) 
{

    return Animate_FinishedGenAnimation(id, move_objects);

}

bool Animate_FinishedMoveGroupAnimation(unsigned int group_id)
{

    return Animate_FinishedGenGroupAnimation(group_id, move_objects);
}

void Animate_DisableByLayer(unsigned char layer_index)
{

    Animate_DisableGenByLayer(layer_index, move_objects);

}