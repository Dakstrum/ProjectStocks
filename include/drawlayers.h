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

DrawObject *CreateNewDrawObject();
DrawObject *CreateScrollBoxObject();
DrawObject *GetDrawObject(int layer, int object);
int RemoveDrawObject(DrawObject *object);
bool DoesObjectExistInCurrentDrawLayer(char *object_name);

void SetTextContent(DrawObject *object, const char *str, ...);

DrawObject** GetAllDrawObjectsInCurrentLayer();
void SetActiveTextBox(DrawObject *object);
DrawObject *GetActiveTextBox();
char *GetTextFromTextBox(char *object_name);

DrawObjectTypeCollection *GetObjectsByType(DrawType type);
void DisposeDrawObjectTypeCollection(DrawObjectTypeCollection *collection);
#endif