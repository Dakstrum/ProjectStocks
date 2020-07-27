#ifndef MANAGER_H
#define MANAGER_H

#include "drawobject.h"

typedef struct Manager {

	void *state;
	void (*Update)(void *state);
	void (*Delete)(void *state);

} Manager;

Manager *Manager_Create();

#endif