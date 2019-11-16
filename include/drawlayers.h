#ifndef DRAWLAYERS_H
#define DRAWLAYERS_H

#include <time.h>

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_video.h>

#include "shared.h"

typedef enum DrawType 
{

    MENU = 0,
    BUTTON,
    POPUP,
    VIDEO,
    TEXT,
    GRAPH,
    TEXTBOX

} DrawType;

typedef struct TextStyle {

    char *font_path;
    ALLEGRO_COLOR color;
    ALLEGRO_FONT *font;

    int font_size;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

} TextStyle;

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

    bool should_repeat;
    bool start_video_immediately;
    ALLEGRO_VIDEO *video;

} Video;

typedef struct Point {

    int x;
    int y;

} Point;

typedef struct Graph {

    char *company;
    TimeSpan timespan;

    Point *points;

    unsigned int num_points;

    ALLEGRO_COLOR color;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

    time_t next_refresh;

} Graph;

typedef struct PopUp {

    ALLEGRO_BITMAP *popup_bitmap;
    long diff_time_to_animate;
    long diff_time_to_stay;

    long diff_time_left_to_animate;
    long diff_time_left_to_stay;

    struct timespec last_animation_time;
    struct timespec last_stay_time;

    float start_x;
    float start_y;
    float current_x;
    float current_y;
    float dx;
    float dy;

    bool done_intro_animation;
    bool done_outro_animation;
    bool done_staying;

    char *direction;
    void (*set_dx_dy)(void *, int, int);
    void (*set_dx_dy_reverse)(void *, int, int);

} PopUp;

typedef struct TextBox {

    ALLEGRO_BITMAP *bitmap;

    bool active;
    bool accept_alphabet_characters;
    bool accept_number_characters;
    // current_character init to -1
    int current_character;
    int limit_characters_to;
    char text[128];
    char *placeholder_text;

    struct timespec flicker;
    bool flicker_drawing;

    TextStyle *text_style;
    TextStyle *placeholder_style;

} TextBox;

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
        PopUp  popup;
        Video  video;
        Text   text;
        Graph graph;
        TextBox textbox;

    };

} DrawObject;

typedef struct MenuWithChilds 
{

    DrawObject *menu;

    int num_buttons;
    int num_text;
    int num_text_boxes;

    int draw_layer;

    DrawObject **buttons;
    DrawObject **text;
    DrawObject **text_boxes;

} MenuWithChilds;


void InitializeDrawLayers(ALLEGRO_DISPLAY *active_display);
int GetMaxObjectsPerDrawLayer();
int CreateNewDrawLayer();
int GetCurrentDrawLayer();

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

DrawObject** GetAllDrawObjectsInCurrentLayer();
void SetActiveTextBox(DrawObject *object);
DrawObject *GetActiveTextBox();
char *GetTextFromTextBox(char *object_name);
#endif