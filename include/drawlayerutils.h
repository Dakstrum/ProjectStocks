#ifndef DRAWLAYERUTILS_H
#define DRAWLAYERUTILS_H


DrawObject     *GetJSONObjectAndAddToDrawLayer(char* object_name);
MenuWithChilds *GetJSONMenuAndAddToDrawLayer(char* menu_name);

DrawObject *GetCompaniesScrollbox(int x, int y, void (*click)(char *scroll_box_content, unsigned short int index));
void InitScrollboxVectors(DrawObject *object);

#endif