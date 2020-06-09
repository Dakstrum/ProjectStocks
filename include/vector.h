#ifndef VECTOR_H
#define VECTOR_H

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

void Vector_Delete(Vector *vector);
void Vector_DeletePtrs(Vector *vector);

#endif