#include <stdlib.h>

#include "mempool.h"
#include "drawobject.h"

static MemPool *mem = NULL;

void DrawObjects_Initialize() 
{

	mem = MemPool_Create(sizeof(DrawObject), 4);

	DrawObject *object = MemPool_Get(mem);

	assert(object != NULL);

	object->name         = NULL;
    object->asset_path   = NULL;
    object->child_of     = NULL;
    object->type         = -1;
    object->bit_flags    = SHOULD_BE_DRAWN;
    object->layer_index  = 0;
    object->object_index = 0;

    MemPool_Free(mem, object);

}

void *DrawObjects_GetMemPoolMem()
{

	return MemPool_Get(mem);

}

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

