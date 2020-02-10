#include <stdlib.h>

#include "drawobject.h"

DrawObject *CreateNewDrawObject() 
{

    DrawObject *object   = malloc(sizeof(DrawObject));
    object->name         = NULL;
    object->asset_path   = NULL;
    object->child_of     = NULL;
    object->type         = -1;
    object->bit_flags    = SHOULD_BE_DRAWN;
    object->layer_index  = 0;
    object->object_index = 0;

    return object;

}

