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

#endif