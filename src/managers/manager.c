
#include <stdlib.h>

#include "manager.h"

Manager *Manager_Create()
{

	Manager *manager = malloc(sizeof(Manager));
	manager->update  = NULL;
	manager->objects = NULL;

	return manager;

}