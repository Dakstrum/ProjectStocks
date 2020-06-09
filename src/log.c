#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <stdbool.h>

#include <sqlite3.h>
#include <allegro5/allegro.h>

#include "log.h"
#include "dbutils.h"
#include "vector.h"
#include "queue.h"
#include "shared.h"


static Queue *log_queue = NULL;
static ALLEGRO_THREAD *queue_thread = NULL;

void ResetQueue();
void *LoggingEntry(ALLEGRO_THREAD *thread, void *arg);

void InitializeLogging() 
{

    log_queue    = Queue_Create();
    queue_thread = al_create_thread(&LoggingEntry, NULL);
    al_start_thread(queue_thread);

}

void WriteQueue() 
{

    Vector *vector = Queue_GetLockFreeVector(log_queue);

    sqlite3 *db;
    if (sqlite3_open_v2("log.db", &db, SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK)
        return;

    ExecuteTransaction(db, vector);
    Vector_DeletePtrs(vector);

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
    Queue_PushMessage(log_queue, GetFormattedPointer("INSERT INTO LOGS (TimeStamp, Log) VALUES ('%s','%s');", current_time, str));

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

    Queue_PushMessage(log_queue, GetFormattedPointer("INSERT INTO LOGS (TimeStamp, Log) VALUES ('%s','%s');", current_time, buffer));

}

void CleanUpLogging() 
{
    al_join_thread(queue_thread, NULL);
    al_destroy_thread(queue_thread);

    Queue_Delete(log_queue);
}

void *LoggingEntry(ALLEGRO_THREAD *thread, void *arg) 
{

    while (!ShouldICleanUp()) {

        al_rest(1.0);
        WriteQueue();

    }
    return NULL;

}