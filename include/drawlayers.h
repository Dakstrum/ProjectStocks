#ifndef DRAWLAYERS_H
#define DRAWLAYERS_H

#include <allegro5/allegro_video.h>

typedef enum DrawType 
{

    MENU = 0,
    BUTTON,
    POPUP,
    VIDEO

} DrawType;

typedef struct Button 
{

    void (*Callback)();
    char *picture_path;
    ALLEGRO_BITMAP *button_bitmap;

} Button;

typedef struct Menu {

    char picture_path[256];
    ALLEGRO_BITMAP *menu_bitmap;

    int num_buttons;
    Button *buttons;

} Menu;

typedef struct Video {

    char video_path[256];

    ALLEGRO_VIDEO *video;

} Video;


typedef struct DrawObject 
{

    DrawType type;
    bool should_this_be_drawn;
    bool scale_to_entire_screen;
    float x;
    float y;
    float width;
    float height;

    union {

        Menu   menu;
        Button button;
        Video  video;

    } member;

} DrawObject;

void InitializeDrawLayers(ALLEGRO_DISPLAY *active_display);
// Returns true if successful, false otherwise.
int CreateNewDrawLayer();

void ClearDrawLayers();
void ClearCurrentDrawLayer();

int AddButtonToDrawLayer(DrawObject *object);
int AddMenuToDrawLayer(DrawObject *object);
int AddVideoToDrawLayer(DrawObject *object);

void DrawLayers();

DrawObject *CreateNewDrawObject();
DrawObject *GetDrawObject(int layer, int object);

void HandleMouseClickInButtonAreas(int x, int y);

#endif