#ifndef MANAGER_H
#define MANAGER_H

#include "drawobject.h"

typedef struct Manager {

	DrawObject **objects;
	void (*update)(struct Manager *manager);

} Manager;

Manager *Manager_Create();

#endif