#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdint.h>

typedef struct Vector {

    void *elements;
    size_t size_of_single_elem;
    size_t initial_mem_size;
    size_t num_elements;
    size_t mem_size;

} Vector;

Vector *Vector_Create(size_t size_of_single_elem, size_t initial_mem_size);
// Pushes back stack allocated memory. Will copy entire value, byte by byte
void Vector_PushBack(Vector *vector, void *element);
// Pushes back pointer
void Vector_PushBackPtr(Vector * vector, void *element);

void Vector_Remove(Vector *vector, unsigned int element_idx);
void Vector_RemovePrt(Vector *vector, unsigned int element_idx);

void *Vector_Last(Vector *vector);
void *Vector_LastPtr(Vector *vector);

Vector *Vector_Concat(Vector *vec_1, Vector *vec_2);
Vector *Vector_GetCopy(Vector *vector);
Vector *Vector_GetSubVector(Vector *vector, size_t start_idx, size_t end_idx);
Vector *Vector_GetSubVectorRef(Vector *vector, size_t start_idx, size_t end_idx);

void Vector_Reset(Vector *vector);
void Vector_Delete(Vector *vector);
void Vector_DeletePtrs(Vector *vector);

void Vector_ForEach(Vector *vector, void *(callback)(void *, uint32_t i));
#endif