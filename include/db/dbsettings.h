#ifndef DBSETTINGS_H
#define DBSETTINGS_H

typedef enum ScreenSetting {

    FULLSCREEN,
    WINDOWED,
    BORDERLESS

} ScreenSetting;

typedef struct WindowSettings {

    int width;
    int height;
    float fps;
    ScreenSetting screen_flag;
    //int fullscreen;
    
} WindowSettings;

WindowSettings GetWindowSettingsFromDB();
void SetWindowResolutionSettings(int width, int height);
void SetFullScreenSettings(int arg);

#endif