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

} Button;


typedef struct DrawObject 
{

    DrawType type;
    union {

        struct {

            float x;
            float y;
            float width;
            float height;
            char picture_path[256];

            int num_buttons;
            Button *buttons;

        } menu;

        Button button;

    } member;

} DrawObject;

void CreateNewDrawLayer();
void ClearDrawLayers();
void ClearCurrentDrawLayer();
void InitializeDrawLayers();
void HandleMouseClickInButtonAreas(int x, int y);

void DrawLayers();

#endif