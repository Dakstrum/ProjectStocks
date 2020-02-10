
#include "draw.h"
#include "cache.h"
#include "drawobject.h"

void InitMenu(DrawObject *object)
{

    object->menu.menu_bitmap = GetBitmapFromCache(object->asset_path);

}

void DrawMenu(DrawObject *object) 
{

    if (object->width != 0.0f && object->height != 0.0f)
        DrawGenericWithWidth(object->menu.menu_bitmap, object->x, object->y, object->width, object->height);
    else
        DrawGeneric(object->menu.menu_bitmap, object->x, object->y);

}

void CleanUpMenu(DrawObject *object) 
{


}
