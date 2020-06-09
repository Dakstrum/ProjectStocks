#include <stdlib.h>

#include "vector.h"

Vector *Vector_Create(size_t size_of_single_elem, size_t initial_mem_size)
{

    Vector *vector = malloc(sizeof(Vector));

    vector->size_of_single_elem = size_of_single_elem;
    vector->num_elements        = 0;
    vector->initial_mem_size    = initial_mem_size;
    vector->mem_size            = initial_mem_size;
    vector->elements            = malloc(size_of_single_elem * initial_mem_size);


    return vector;

}

void Vector_PushBack(Vector *vector, void *element)
{

    if (vector->num_elements == vector->mem_size) {

        vector->mem_size += vector->initial_mem_size;
        vector->elements  = realloc(vector->elements, vector->size_of_single_elem * vector->mem_size);

    }

    unsigned char *temp        = (unsigned char *)vector->elements;
    unsigned char *new_element = (unsigned char *)element;

    for (size_t i = 0; i < vector->size_of_single_elem;i++)
        *(temp + vector->num_elements * vector->size_of_single_elem + i) = *(new_element + i);

    vector->num_elements++;

}

void Vector_PushBackPtr(Vector *vector, void *element)
{

    if (vector->num_elements == vector->mem_size) {

        vector->mem_size += vector->initial_mem_size;
        vector->elements  = realloc(vector->elements, vector->size_of_single_elem * vector->mem_size);

    }

    void **elements = vector->elements;
    elements[vector->num_elements] = element;
    vector->num_elements++;

}

void Vector_DeletePtrs(Vector *vector)
{
    void **array = (void **)vector->elements;

    for (size_t i = 0;i < vector->num_elements;i++)
        free(array[i]);

    vector->num_elements = 0;
    free(vector);

}

void Vector_Delete(Vector *vector)
{

    free(vector->elements);
    free(vector);

}