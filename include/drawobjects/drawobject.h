#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H

#include <time.h>

#include <allegro5/allegro_video.h>
#include <allegro5/allegro_font.h>

#include "shared.h"

typedef struct RGBA {

    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

} RGBA;

typedef enum DrawType 
{

    MENU = 0,
    BUTTON,
    POPUP,
    VIDEO,
    TEXT,
    GRAPH,
    TEXTBOX,
    SCROLLBOX

} DrawType;

typedef enum DrawBitFlags 
{

    SHOULD_BE_DRAWN                    = 1,
    TEXT_IS_DYNAMIC                    = 2,
    POPUP_DONE_INTRO_ANIMATION         = 4,
    POPUP_DONE_OUTRO_ANIMATION         = 8,
    POPUP_DONE_STAYING                 = 16,
    TEXTBOX_ACTIVE                     = 32,
    TEXTBOX_ACCEPT_ALPHABET_CHARACTERS = 64,
    TEXTBOX_ACCEPT_NUMBER_CHARACTERS   = 128,
    TEXTBOX_FLICKER_DRAWING            = 256,
    VIDEO_SHOULD_REPEAT                = 512,
    VIDEO_SHOULD_START_IMMEDIATELY     = 1024,
    BUTTON_MOUSE_HOVERING              = 2048,
    CAN_BE_REMOVED                     = 4096,
    BUTTON_HAS_TINT                    = 8192

} DrawBitFlags;

typedef struct TextStyle {

    char *font_path;
    ALLEGRO_COLOR color;
    ALLEGRO_FONT *font;

    unsigned char font_size;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

} TextStyle;

typedef struct Point {

    unsigned short int x;
    unsigned short int y;

} Point;

typedef struct NewsList {

    ALLEGRO_BITMAP *button_bitmap;
    unsigned short num_elements;
    char **elements;
    
} NewsList;


typedef struct Graph {

    char *company;
    TimeSpan timespan;

    Point *points;

    unsigned short int num_points;

    ALLEGRO_COLOR color;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

    struct timespec next_refresh;

} Graph;

typedef struct Button 
{

    void (*Callback)();
    ALLEGRO_BITMAP *button_bitmap;
    RGBA *tint_color;

} Button;


typedef struct Menu 
{

    ALLEGRO_BITMAP *menu_bitmap;

} Menu;

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

    char text[128];
    char *direction;
    TextStyle *text_style;
    void (*set_dx_dy)(void *, int, int);
    void (*set_dx_dy_reverse)(void *, int, int);

} PopUp;

typedef struct ScrollBox {

    ALLEGRO_BITMAP *boxes_bitmap;
    short int vertical_spacing;
    short int vertical_offset;
    short int min_vertical_offset;
    short int max_vertical_offset;
    unsigned short int box_width;
    unsigned char box_height;

    void (*box_click)(char *, unsigned short int);

    unsigned short int num_items;
    short int currently_tinted;
    char **text_content;
    char **icon_paths;

    ALLEGRO_BITMAP **icons;
    TextStyle *text_style;

} ScrollBox;

typedef struct Text 
{

    char *content;
    ALLEGRO_COLOR color;
    ALLEGRO_FONT *font;

    unsigned char font_size;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

} Text;

typedef struct TextBox {

    ALLEGRO_BITMAP *bitmap;

    // current_character init to -1
    short int current_character;
    short int limit_characters_to;
    char text[128];
    char *placeholder_text;

    struct timespec flicker;

    TextStyle *text_style;
    TextStyle *placeholder_style;

} TextBox;

typedef struct Video {

    ALLEGRO_VIDEO *video;

} Video;


typedef struct DrawObject 
{

    DrawType type;
    float x;
    float y;
    float width;
    float height;

    unsigned char layer_index;
    unsigned char object_index;
    unsigned short int bit_flags;

    char *name;
    char *asset_path;
    char *child_of;

    union {

        Menu      menu;
        Button    button;
        PopUp     popup;
        Video     video;
        Text      text;
        Graph     graph;
        TextBox   textbox;
        ScrollBox scrollbox;

    };

} DrawObject;

typedef struct MenuWithChilds 
{

    DrawObject *menu;

    unsigned char num_buttons;
    unsigned char num_text;
    unsigned char num_text_boxes;
    unsigned char num_scroll_boxes;
    unsigned char draw_layer;

    DrawObject **buttons;
    DrawObject **text;
    DrawObject **text_boxes;
    DrawObject **scroll_boxes;

} MenuWithChilds;

typedef struct DrawObjectTypeCollection {

    DrawObject **objects;
    unsigned short int num_objects;

} DrawObjectTypeCollection;

DrawObject *CreateNewDrawObject();

#endif