#ifndef DRAWLAYERS_H
#define DRAWLAYERS_H

#include "drawobject.h"
#include "shared.h"


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

DrawObject *GetDrawObject(int layer, int object);
DrawObject *GetDrawObjectByName(char *object_name);
int RemoveDrawObject(DrawObject *object);
bool DoesObjectExistInCurrentDrawLayer(char *object_name);

DrawObject** GetAllDrawObjectsInCurrentLayer();
void SetActiveTextBox(DrawObject *object);
DrawObject *GetActiveTextBox();
char *GetTextFromTextBox(char *object_name);

DrawObjectTypeCollection *GetObjectsByType(DrawType type);
DrawObject *GetObjectAndDraw(char* object_name);
void DisposeDrawObjectTypeCollection(DrawObjectTypeCollection *collection);

RGBA *GetRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

#endif