#include <assert.h>
#include <stdint.h>
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

    unsigned char *temp        = vector->elements;
    unsigned char *new_element = element;

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

Vector *Vector_GetCopy(Vector *vector)
{

    size_t num_elements = vector->num_elements;
    size_t size_of_single_elem = vector->size_of_single_elem;

    Vector *vec = malloc(sizeof(Vector));

    vec->size_of_single_elem = vector->size_of_single_elem;
    vec->num_elements        = vector->num_elements;
    vec->initial_mem_size    = vector->initial_mem_size;
    vec->mem_size            = vector->mem_size;
    vec->elements            = malloc(size_of_single_elem * vector->mem_size);

    uint8_t *new_vec = vec->elements;
    uint8_t *old_vec = vector->elements;

    for (size_t i = 0;i < num_elements;i++)
        for (size_t j = 0;j < size_of_single_elem;j++)
            *(new_vec + i * size_of_single_elem + j) = *(old_vec + i * size_of_single_elem + j);

    return vec;

}

void *Vector_GetLast(Vector *vector)
{

    uint8_t *elements = vector->elements;
    return (elements + (vector->size_of_single_elem * (vector->num_elements - 1)));

}

Vector *Vector_GetSubVector(Vector *vector, size_t start_idx, size_t end_idx) 
{

    assert(end_idx > start_idx);
    assert(end_idx <= vector->num_elements);

    size_t size_of_single_elem = vector->size_of_single_elem;
    Vector *vec = malloc(sizeof(Vector));

    vec->size_of_single_elem = size_of_single_elem;
    vec->num_elements        = end_idx - start_idx;
    vec->initial_mem_size    = vector->initial_mem_size;
    vec->mem_size            = end_idx - start_idx;
    vec->elements            = malloc(size_of_single_elem * vector->mem_size);

    uint8_t *new_vec = vec->elements;
    uint8_t *old_vec = vector->elements;

    for (size_t i = start_idx;i < end_idx;i++)
        for (size_t j = 0;j < size_of_single_elem;j++)
            *(new_vec + (i - start_idx) * size_of_single_elem + j) = *(old_vec + i * size_of_single_elem + j);

    return vec;

}

Vector *Vector_GetSubVectorRef(Vector *vector, size_t start_idx, size_t end_idx) 
{

    assert(end_idx > start_idx);
    assert(end_idx <= vector->num_elements);

    size_t size_of_single_elem = vector->size_of_single_elem;
    Vector *vec = malloc(sizeof(Vector));

    vec->size_of_single_elem = size_of_single_elem;
    vec->num_elements        = end_idx - start_idx;
    vec->initial_mem_size    = vector->initial_mem_size;
    vec->mem_size            = end_idx - start_idx;
    vec->elements            = vector->elements;

    vec->elements += start_idx * size_of_single_elem;

    return vec;

}

Vector *Vector_Concat(Vector *vec_1, Vector *vec_2) 
{

    assert(vec_1->size_of_single_elem == vec_2->size_of_single_elem);

    uint32_t vec_1_num_elem    = vec_1->num_elements;
    uint32_t vec_2_num_elem    = vec_2->num_elements;
    size_t size_of_single_elem = vec_1->size_of_single_elem;;

    Vector *vector = Vector_Create(size_of_single_elem, vec_1_num_elem + vec_2_num_elem + 1);

    uint8_t *vector_elem = vector->elements;
    uint8_t *vec_1_elem  = vec_1->elements;
    uint8_t *vec_2_elem  = vec_2->elements;

    for (size_t i = 0; i < vec_1_num_elem;i++)
        for (size_t j = 0;j < size_of_single_elem;j++)
            *(vector_elem + i * size_of_single_elem + j) = *(vec_1_elem + i * size_of_single_elem + j);

    for (size_t i = vec_1_num_elem;i < vec_1_num_elem + vec_2_num_elem;i++)
        for (size_t j = 0;j < size_of_single_elem;j++)
            *(vector_elem + i * size_of_single_elem + j) = *(vec_2_elem + (i - vec_1_num_elem) * size_of_single_elem + j);

    return vector;

}

void Vector_Reset(Vector *vector)
{

    vector->num_elements = 0;

}

void Vector_DeletePtrs(Vector *vector)
{
    void **array = (void **)vector->elements;

    for (size_t i = 0;i < vector->num_elements;i++)
        free(array[i]);

    vector->num_elements = 0;
    free(vector);

}

void Vector_Remove(Vector *vector, unsigned int element_idx)
{

    assert(element_idx < vector->num_elements);

    unsigned char *elements = vector->elements;
    for (size_t i = element_idx; i < vector->num_elements-1;i++)
        for (size_t j = 0; j < vector->size_of_single_elem; j++)
            *(elements + i * vector->size_of_single_elem + j) = *(elements + (i + 1) * vector->size_of_single_elem + j);

    vector->num_elements--;
}

void Vector_RemovePrt(Vector *vector, unsigned int element_idx)
{

    assert(element_idx < vector->num_elements);

    void **elements = vector->elements;

    for (size_t i = element_idx; i < vector->num_elements-1; i++)
        elements[i] = elements[i+1];

    vector->num_elements--;

}

void Vector_Delete(Vector *vector)
{

    free(vector->elements);
    free(vector);

}