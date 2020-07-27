
#include <stdlib.h>

#include "manager.h"

Manager *Manager_Create()
{

	Manager *manager = malloc(sizeof(Manager));
	manager->Update  = NULL;
	manager->Delete  = NULL;
	manager->state   = NULL;

	return manager;

}