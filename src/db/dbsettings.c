
#include <stdlib.h>

#include "dbsettings.h"
#include "dbutils.h"

int SetWindowSettingsIfExists(void *settings, int argc, char **argv, char **col_name) 
{

    WindowSettings *temp_settings = (WindowSettings *)settings;
    if (argc == 0)
        return 0;

    temp_settings->width          = atoi(argv[0]);
    temp_settings->height         = atoi(argv[1]);
    temp_settings->screen_flag    = atoi(argv[2]);

    return 0;

}

WindowSettings GetWindowSettingsFromDB() 
{

    WindowSettings settings = {1920, 1080, WINDOWED};
    ExecuteQueryF(&SetWindowSettingsIfExists, &settings, "SELECT WindowWidth, WindowHeight, WindowStyle FROM Settings");
    return settings;

}

void SetWindowResolutionSettings(int width, int height)
{

    ExecuteQueryF(NULL, NULL,"UPDATE Settings SET WindowWidth = %d, WindowHeight = %d WHERE SettingsId = 1;", width, height );

}