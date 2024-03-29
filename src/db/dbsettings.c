
#include <stdlib.h>

#include "dbsettings.h"
#include "dbutils.h"

int SetWindowSettingsIfExists(void *settings, int argc, char **argv, char **col_name) 
{

    WindowSettings *temp_settings = (WindowSettings *)settings;
    if (argc == 0)
        return 0;

    temp_settings->width       = atoi(argv[0]);
    temp_settings->height      = atoi(argv[1]);
    temp_settings->screen_flag = atoi(argv[2]);
    temp_settings->fps         = atof(argv[3]);
    temp_settings->fullscreen  = atof(argv[4]);

    return 0;

}

WindowSettings GetWindowSettingsFromDB() 
{

    WindowSettings settings = {1920, 1080, WINDOWED, 0};
    ExecuteQueryF(&SetWindowSettingsIfExists, &settings, "SELECT WindowWidth, WindowHeight, WindowStyle, FPS, FullScreen FROM Game_Settings");
    return settings;

}

void SetWindowResolutionSettings(int width, int height)
{

    ExecuteQueryF(NULL, NULL,"UPDATE Game_Settings SET WindowWidth = %d, WindowHeight = %d WHERE SettingsId = 1;", width, height );

}

void SetFullScreenSettings(int arg)
{

    ExecuteQueryF(NULL, NULL,"UPDATE Game_Settings SET FullScreen = %d WHERE SettingsId = 1;", arg );

}

int GetAudioMasterSetting()
{

    return 1;
}

int GetAudioMusicSetting()
{

    return 1;
}

int GetAudioFxSetting()
{

    return 1;
}