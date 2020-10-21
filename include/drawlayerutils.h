#ifndef DRAWLAYERUTILS_H
#define DRAWLAYERUTILS_H


DrawObject     *GetJSONObjectAndAddToDrawLayer(char* object_name);
MenuWithChilds *GetJSONMenuAndAddToDrawLayer(char* menu_name);

DrawObject *CreateCompaniesScrollbox(int x, int y, void (*click)(char *scroll_box_content, unsigned short int index));
DrawObject *CreateCategoryScrollbox(int x, int y, void (*click)(char *scroll_box_content, unsigned short int index));
DrawObject *CreateCategorySpecificCompaniesScrollbox(int x, int y, int cat_id, void (*click)(char *scroll_box_content, unsigned short int index));

DrawObject *GetCategoryScrollbox();
DrawObject *GetCategorySpecificCompaniesScrollbox();

void InitScrollboxVectors(DrawObject *object);

#endif