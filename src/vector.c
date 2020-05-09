#include <stdlib.h>
#include <stdint.h>

#include "vector.h"

Vector *CreateVector(int size_of_single_elem, int initial_mem_size)
{

    Vector *vector = malloc(sizeof(Vector));

    vector->size_of_single_elem = size_of_single_elem;
    vector->num_elements        = 0;
    vector->initial_mem_size    = initial_mem_size;
    vector->mem_size            = initial_mem_size;
    vector->elements            = malloc(size_of_single_elem * initial_mem_size);


    return vector;

}

void PushBack(Vector *vector, void *element)
{

    if (vector->num_elements == vector->mem_size) {

        vector->mem_size += vector->initial_mem_size;
        vector->elements  = realloc(vector->elements, vector->size_of_single_elem * vector->mem_size);

    }
    
    unsigned char *temp        = (unsigned char *)vector->elements;
    unsigned char *new_element = (unsigned char *)element;

    for (int i = 0; i < vector->size_of_single_elem;i++)
        *(temp + vector->num_elements * vector->size_of_single_elem + i) = *(new_element + i);

    vector->num_elements += 1;

}

void FreeVectorPtrElements(Vector *vector)
{
    uintptr_t ptr           = 0;
    unsigned char byte      = 0;
    unsigned char *array    = (unsigned char *)vector->elements;
    int size_of_single_elem = vector->size_of_single_elem;

    for (int i = 0;i < vector->num_elements;i++) {

        for (int j = 0; j < size_of_single_elem;j++) {
            
            byte  = array[i * size_of_single_elem + j];
            ptr   = ptr << 8;
            ptr  |= byte;

        }

        void **to_be_freed = (void **)ptr;
        free(*to_be_freed);
        ptr = 0;

    }

}

void DeleteVector(Vector *vector)
{

    free(vector->elements);
    free(vector);

}


void DeleteJustVector(Vector *vector)
{

    free(vector);

}