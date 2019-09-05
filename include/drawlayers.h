#ifndef DRAWLAYERS_H
#define DRAWLAYERS_H

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_video.h>

typedef enum DrawType 
{

    MENU = 0,
    BUTTON,
    POPUP,
    VIDEO,
    TEXT

} DrawType;

typedef struct Text 
{

    char *text;
    ALLEGRO_COLOR color;
    ALLEGRO_FONT *font;

    int font_size;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

} Text;

typedef struct Button 
{

    void (*Callback)();
    ALLEGRO_BITMAP *button_bitmap;

} Button;

typedef struct Menu {

    ALLEGRO_BITMAP *menu_bitmap;

} Menu;

typedef struct Video {

    ALLEGRO_VIDEO *video;

} Video;

typedef struct DrawObject 
{

    DrawType type;
    bool should_this_be_drawn;
    float x;
    float y;
    float width;
    float height;

    int layer_index;
    int object_index;

    char *name;
    char *asset_path;

    union {

        Menu   menu;
        Button button;
        Video  video;
        Text   text;

    } member;

} DrawObject;


void InitializeDrawLayers(ALLEGRO_DISPLAY *active_display);
int CreateNewDrawLayer();

void ClearDrawLayers();
void ClearCurrentDrawLayer();

int AddButtonToDrawLayer(DrawObject *object);
int AddMenuToDrawLayer(DrawObject *object);
int AddVideoToDrawLayer(DrawObject *object, bool start_video_immediately);
int AddTextToDrawLayer(DrawObject *object);

void DrawLayers();

DrawObject *CreateNewDrawObject();
DrawObject *GetDrawObject(int layer, int object);
int RemoveDrawObject(DrawObject *object);

void HandleMouseClickInButtonAreas(int x, int y);

#endif