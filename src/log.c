#include <stdio.h>
#include <sqlite3.h>

void Log(const char *str) 
{

    sqlite3 *db;
    sqlite3_stmt *res;
    if (sqlite3_open_v2("log.db", &db, SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_READWRITE, NULL) == SQLITE_OK) {

        char *logger = "INSERT INTO LOGS (Log) VALUES (@log);";
        if (sqlite3_prepare_v2(db, logger, -1, &res, 0) == SQLITE_OK) {

            sqlite3_bind_text(res, sqlite3_bind_parameter_index(res, "@log"), str, -1, 0);
            sqlite3_step(res);

        }

    }
    sqlite3_finalize(res);
    sqlite3_close(db);

}