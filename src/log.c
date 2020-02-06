#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <stdbool.h>

#include <sqlite3.h>
#include <allegro5/allegro.h>

#include "log.h"
#include "shared.h"

typedef struct Queue 
{

    char *message;

} Queue;

static Queue *queue                         = NULL;
static Queue *exclusive_queue               = NULL;
static const unsigned int max_queue_size    = 128;
static unsigned int queue_pointer           = 0;
static unsigned int exclusive_queue_pointer = 0;
static ALLEGRO_MUTEX *log_mutex             = NULL;
static ALLEGRO_THREAD *queue_thread         = NULL;

void ResetQueue();
void *LoggingEntry(ALLEGRO_THREAD *thread, void *arg);

void InitializeLogging() 
{

    log_mutex = al_create_mutex();
    if (log_mutex == NULL){

        LogNoQueue("Could not create log mutex");
        SetCleanUpToTrue();
        return;

    }

    queue           = malloc(sizeof(Queue) * max_queue_size);
    exclusive_queue = malloc(sizeof(Queue) * max_queue_size);
    for (size_t i = 0;i < max_queue_size; i++) {
        queue[i].message           = NULL;
        exclusive_queue[i].message = NULL;
    }

    queue_thread = al_create_thread(&LoggingEntry, NULL);
    al_start_thread(queue_thread);

}

void InsertMessage(sqlite3 *db, int queue_message_idx) 
{

    if (exclusive_queue[queue_message_idx].message == NULL)
        return;

    char *error = NULL;
    sqlite3_exec(db, exclusive_queue[queue_message_idx].message, NULL, NULL, &error);
    if (error != NULL)
        LogFNoQueue("SQL ERROR %s, query = %s", error, exclusive_queue[queue_message_idx].message);

}

void WriteQueue() 
{

    sqlite3 *db;
    if (sqlite3_open_v2("log.db", &db, SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK)
        return;

    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, 0, 0);

    for (size_t i = 0; i < exclusive_queue_pointer; i++)
        InsertMessage(db, i);

    sqlite3_exec(db, "END TRANSACTION", NULL, 0, 0);
    sqlite3_close(db);

}

void LogNoQueue(const char *str) 
{

    sqlite3 *db;
    if (sqlite3_open_v2("log.db", &db, SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_READWRITE, NULL) == SQLITE_OK) {

        sqlite3_stmt *res;
        char *logger = "INSERT INTO LOGS (Log) VALUES (@log);";
        if (sqlite3_prepare_v2(db, logger, -1, &res, 0) == SQLITE_OK) {

            sqlite3_bind_text(res, sqlite3_bind_parameter_index(res, "@log"), str, -1, 0);
            sqlite3_step(res);

        }
        sqlite3_finalize(res);

    }
    sqlite3_close(db);

}

void LogFNoQueue(const char *str, ...) 
{

    char buffer[2048];
    va_list args;
    va_start(args, str);
    vsprintf(buffer, str, args);

    LogNoQueue(buffer);
    va_end(args);

}

void SetCurrentTime(char buffer[128]) 
{

    time_t log_time = time(NULL);
    strftime(buffer, 128, "%c", localtime(&log_time));

}

void Log(const char *str) 
{

    char current_time[128];
    SetCurrentTime(current_time);

    al_lock_mutex(log_mutex);

    if(queue_pointer < max_queue_size) {

        queue[queue_pointer].message = GetFormattedPointer("INSERT INTO LOGS (TimeStamp, Log) VALUES ('%s','%s');", current_time, str);
        queue_pointer++;

    }

    al_unlock_mutex(log_mutex);

}

void LogF(const char *str, ...) 
{

    char current_time[128];
    SetCurrentTime(current_time);

    char buffer[1024];
    va_list args;
    va_start(args, str);
    vsprintf(buffer, str, args);
    va_end(args);

    al_lock_mutex(log_mutex);

    if(queue_pointer < max_queue_size) {

        queue[queue_pointer].message = GetFormattedPointer("INSERT INTO LOGS (TimeStamp, Log) VALUES ('%s','%s');", current_time, buffer);
        queue_pointer++;

    }

    al_unlock_mutex(log_mutex);

}

void ResetQueue() 
{

    for (size_t i = 0; i < queue_pointer; i++) {

        if (queue[i].message != NULL)
            free(queue[i].message);
        queue[i].message = NULL;

    }
    queue_pointer = 0;

}

void ResetExclusiveQueue()
{

    for (size_t i = 0; i < exclusive_queue_pointer;i++) {

        if (exclusive_queue[i].message != NULL) { 

            free(exclusive_queue[i].message);
            exclusive_queue[i].message = NULL;

        }

    }
    exclusive_queue_pointer = 0;

}

void CleanUpLogging() 
{
    al_join_thread(queue_thread, NULL);
    al_destroy_thread(queue_thread);

    ResetQueue();
    al_destroy_mutex(log_mutex);
    free(queue);
}

void TransferQueue()
{

    ResetExclusiveQueue();

    al_lock_mutex(log_mutex);

    for (size_t i = 0; i < queue_pointer;i++) {

        exclusive_queue[i].message = queue[i].message;
        queue[i].message           = NULL;

    }
    exclusive_queue_pointer = queue_pointer;
    queue_pointer           = 0;

    al_unlock_mutex(log_mutex);

}

void *LoggingEntry(ALLEGRO_THREAD *thread, void *arg) 
{

    while (!ShouldICleanUp()) {

        al_rest(1.0);
        TransferQueue();
        WriteQueue();

    }
    return NULL;

}