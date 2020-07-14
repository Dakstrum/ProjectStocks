#ifndef MEMPOOL_H
#define MEMPOOL_H

#include "vector.h"

typedef struct MemPool {

	Vector *in_use;
	Vector *pool;

} MemPool;

MemPool *MemPool_Create(size_t size_of_single_elem, size_t initial_mem_size);

void *MemPool_GetFree(MemPool *mem);
void MemPool_Free(MemPool *mem, void *element);

#endif