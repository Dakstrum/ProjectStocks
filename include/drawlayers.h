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

    char *content;
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

    bool start_video_immediately;
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

    char *child_of;

    union {

        Menu   menu;
        Button button;
        Video  video;
        Text   text;

    } member;

} DrawObject;

typedef struct MenuWithChilds 
{

    DrawObject *menu;

    int num_buttons;
    int num_text;

    DrawObject **buttons;
    DrawObject **text;

} MenuWithChilds;


void InitializeDrawLayers(ALLEGRO_DISPLAY *active_display);
int CreateNewDrawLayer();

void ClearDrawLayers();
void ClearCurrentDrawLayer();

int AddObjectToDrawLayer(DrawObject *object);
int AddButtonToDrawLayer(DrawObject *object);
int AddMenuToDrawLayer(DrawObject *object);
int AddMenuWithChildsToDrawLayer(MenuWithChilds *menu_with_childs);
int AddVideoToDrawLayer(DrawObject *object);
int AddTextToDrawLayer(DrawObject *object);

void DrawLayers();

DrawObject *CreateNewDrawObject();
DrawObject *GetDrawObject(int layer, int object);
int RemoveDrawObject(DrawObject *object);
bool DoesObjectExistInCurrentDrawLayer(char *object_name);

void HandleMouseClickInButtonAreas(int x, int y);

#endif