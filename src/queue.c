
#include <stdlib.h>

#include "queue.h"

Queue *Queue_Create()
{

    Queue *temp    = malloc(sizeof(Queue));
    temp->messages = CreateVector(sizeof(char *), 128);
    temp->mutex    = al_create_mutex();
    return temp;

}

void Queue_Lock(Queue *queue)
{

    al_lock_mutex(queue->mutex);

}

void Queue_Unlock(Queue *queue)
{

    al_unlock_mutex(queue->mutex);

}

void Queue_Delete(Queue *queue)
{

    FreeVectorPtrElements(queue->messages);
    free(queue);

}

void Queue_PushMessage(Queue *queue, char *message)
{

    Queue_Lock(queue);
    PushBackPtr(queue->messages, message);
    Queue_Unlock(queue);

}

Vector *Queue_GetLockFreeVector(Queue *queue)
{

    Queue_Lock(queue);
    Vector *previous_messages = queue->messages;
    queue->messages           = CreateVector(sizeof(char *), 128);
    Queue_Unlock(queue);

    return previous_messages;
}
