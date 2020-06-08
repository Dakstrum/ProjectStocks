#ifndef QUEUE_H
#define QUEUE_H

#include <allegro5/allegro.h>

#include "vector.h"

typedef struct Queue
{

    Vector *messages;
    Vector *temp_messages;
    ALLEGRO_MUTEX *mutex;

} Queue;

Queue *Queue_Create();

void Queue_PushMessage(Queue *queue, char *message);
Vector *Queue_GetLockFreeVector(Queue *queue);

#endif