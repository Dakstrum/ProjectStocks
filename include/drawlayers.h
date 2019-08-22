#ifndef DRAWLAYERS_H
#define DRAWLAYERS_H



typedef enum DrawType 
{

    MENU = 0,
    BUTTON,
    POPUP

} DrawType;

typedef struct Button 
{

    float x;
    float y;
    float width;
    float height;
    void (*Callback)();
    char *picture_path;
    ALLEGRO_BITMAP *button_bitmap;

} Button;

typedef struct Menu {

    float x;
    float y;
    float width;
    float height;
    char picture_path[256];

    ALLEGRO_BITMAP *menu_bitmap;

    int num_buttons;
    Button *buttons;

} Menu;

typedef struct Video {

    float x;
    float y;
    float width;
    float height;
    char video_path[256];

} Video;


typedef struct DrawObject 
{

    DrawType type;
    bool should_this_be_drawn;
    bool scale_to_entire_screen;
    union {

        Menu menu;
        Button button;

    } member;

} DrawObject;

void InitializeDrawLayers(ALLEGRO_DISPLAY *active_display);
// Returns true if successful, false otherwise.
bool CreateNewDrawLayer();

void ClearDrawLayers();
void ClearCurrentDrawLayer();

bool AddButtonToDrawLayer(DrawObject *object);
bool AddMenuToDrawLayer(DrawObject *object);

void DrawLayers();

void HandleMouseClickInButtonAreas(int x, int y);

#endif