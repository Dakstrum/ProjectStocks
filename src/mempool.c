
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "mempool.h"
#include "log.h"

MemPool *MemPool_Create(size_t size_of_single_elem, size_t initial_mem_size)
{

	MemPool *mem = malloc(sizeof(MemPool));
	mem->in_use  = Vector_Create(sizeof(bool), initial_mem_size);
	mem->pool    = Vector_Create(sizeof(void *), initial_mem_size);

	for (size_t i = 0; i < initial_mem_size;i++) {

		bool temp = false;
		Vector_PushBack(mem->in_use, &temp);
		Vector_PushBackPtr(mem->pool, malloc(size_of_single_elem));

	}

	return mem;

}

void *MemPool_Get(MemPool *mem)
{

	bool *in_use = mem->in_use->elements;
	for (size_t i = 0; i < mem->in_use->num_elements;i++) {

		if (in_use[i] == false) {

			in_use[i] = true;
			void **elements = mem->pool->elements;
			return elements[i];

		}

	}

	return NULL;

}

void MemPool_Free(MemPool *mem, void *element)
{

	void **elements = mem->pool->elements;
	bool *in_use   = mem->in_use->elements;
	for (size_t i = 0; i < mem->pool->num_elements;i++) {

		if (element == elements[i]) {

			in_use[i] = false;
			break;

		}

	}

}