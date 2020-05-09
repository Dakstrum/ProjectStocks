#ifndef VECTOR_H
#define VECTOR_H

typedef struct Vector {

    void *elements;
    int size_of_single_elem;
    int initial_mem_size;
    int num_elements;
    int mem_size;

} Vector;

Vector *CreateVector(int size_of_single_elem, int initial_mem_size);
void PushBack(Vector * vector, void *element);

// Free's elements that are themselves point to allocated memory.
void FreeVectorPtrElements(Vector *vector);
// Frees both the elements pointer and the vector itself.
void DeleteVector(Vector *vector);
// Frees only the vector itself, leaving the elements untouched
void DeleteJustVector(Vector *vector);

#endif