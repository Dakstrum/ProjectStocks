#include "newsmanager.h"
#include "drawobject.h"

void NewsManager_Update(Manager *manager)
{



}

Manager *NewsManager_Create()
{

	Manager *manager = Manager_Create();
	manager->update  = NewsManager_Update;
	manager->objects = malloc(sizeof(DrawObject *) * 4);
	for (size_t i = 0;i < 4;i++)
		manager->objects[i] = CreateNewDrawObject();

	return manager;

}