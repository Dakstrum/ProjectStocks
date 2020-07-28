#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "log.h"
#include "shared.h"
#include "rendering.h"
#include "window.h"
#include "draw.h"
#include "dbsettings.h"

typedef struct DisplayFlagMap {

    ScreenSetting display_setting;
    int allegro_flag;

} DisplayFlagMap;

static ALLEGRO_DISPLAY *display     = NULL;
static ALLEGRO_BITMAP *video_buffer = NULL;
static WindowSettings window_settings;

static const float window_width  = 1.0/1920.0;
static const float window_height = 1.0/1080.0;

static const int MONITOR_ADAPTER = 0;
static int monitor_width         = 0;
static int monitor_height        = 0;


void Window_SaveResize(int width, int height)
{

    SetWindowResolutionSettings(width, height);
    window_settings.width  = width;
    window_settings.height = height;

}

void Window_Resize(int width, int height) 
{
    if (!al_resize_display(display, width, height)) {

        Log("Could not resize display");
        return;

    }
    Window_SaveResize(width, height);
}

void Window_SetDisplayIcon()
{

    assert(al_is_image_addon_initialized());
    al_set_display_icon(display, al_load_bitmap("assets/icon.tga"));

}

void Window_SetMonitoryResolution(int adapter)
{
    ALLEGRO_MONITOR_INFO info;
    al_get_monitor_info(adapter, &info);

    monitor_width  = info.x2 - info.x1;
    monitor_height = info.y2 - info.y1;
}


void Window_Initialize() 
{
    
    window_settings = GetWindowSettingsFromDB();
    Window_SetMonitoryResolution(MONITOR_ADAPTER);

    if(window_settings.fullscreen) {

        Window_SaveResize(monitor_width, monitor_height);
        al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW | ALLEGRO_OPENGL);

    } else {

        al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_OPENGL);

    }
    
    display = al_create_display(window_settings.width, window_settings.height);
    Window_SetDisplayIcon();

}

WindowSettings GetWindowSettings() 
{

    return window_settings;    

}

WindowScale GetWindowScale() 
{

    WindowScale scale;
    scale.x_scale = (float)window_settings.width * window_width;
    scale.y_scale = (float)window_settings.height * window_height;

    return scale;

}

void HandleWindowEvents(ALLEGRO_EVENT event) 
{

    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        SetCleanUpToTrue();

}

void CleanUpDisplay() 
{

    al_destroy_display(display);

}


ALLEGRO_DISPLAY *GetDisplay() 
{

    assert(display != NULL);
    return display;

}

int Window_Width()
{

    return window_settings.width;

}

int Window_Height() 
{

    return window_settings.height;

}

void Window_InitVideoBuffer()
{

    al_destroy_bitmap(video_buffer);
    video_buffer = al_create_bitmap(1920, 1080);
    al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);

}

void Window_SetVideoBufferAsTarget()
{

    assert(video_buffer != NULL);
    al_set_target_bitmap(video_buffer);

}

void Window_SetVideoBackBuffer()
{

    al_set_target_backbuffer(display);

}

void Window_DrawBackBuffer()
{

    DrawBackBuffer(video_buffer);

}

float Window_FPS()
{

    return window_settings.fps;

}

void Window_FullScreen()
{
    
    window_settings = GetWindowSettingsFromDB();

    if (!window_settings.fullscreen) {

        al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, true);
        Window_SaveResize(monitor_width, monitor_height);
        SetFullScreenSettings(1);

    } else {

        al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, false);
        SetFullScreenSettings(0);

    }

}