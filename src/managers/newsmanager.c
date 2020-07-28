#include "newsmanager.h"
#include "animations.h"
#include "text.h"
#include "log.h"

#include "drawobject.h"
#include "drawlayers.h"
#include "window.h"

typedef enum NewsManagerStates {

	NOT_ANIMATING,
	PULL_RIGHT,
	PUSH_DOWN,
	PUSH_LEFT

} NewsManagerStates;

typedef struct NewsManager {

	DrawObject **objects;
	unsigned char news_state;
	unsigned int animation_id;
	float x_i;
	float y_i;

} NewsManager;


void NewsManager_CheckForNews()
{



}

void NewsManager_Insert()
{



}

//unsigned int Animate_MoveDrawObject(DrawObject *object, float n_x, float n_y, long animation_length);

void NewsManager_Transition(NewsManager *manager)
{

	switch (manager->news_state) {

		case PULL_RIGHT:
			manager->news_state = PUSH_DOWN;
			break;
		case PUSH_DOWN:
			manager->news_state = PUSH_LEFT;
			break;
		case PUSH_LEFT:
			manager->news_state = NOT_ANIMATING;
			break;
		case NOT_ANIMATING:
			manager->news_state = PULL_RIGHT;
			break;

	}

}

void NewsManager_PullRight(NewsManager *manager)
{

	manager->animation_id = Animate_MoveDrawObject(manager->objects[2], manager->objects[2]->x+1000, manager->objects[2]->y, 500);

}

void NewsManager_PushDown(NewsManager *manager)
{

	DrawObject **objects = manager->objects;

	objects[2]->x = objects[1]->x;
	objects[2]->y = objects[1]->y; 

	objects[1]->x = objects[0]->x;
	objects[1]->y = objects[0]->y; 

	objects[0]->x = manager->x_i+1000;
	objects[0]->y = manager->y_i;

	manager->animation_id = Animate_MoveDrawObject(manager->objects[1], manager->objects[1]->x, manager->objects[1]->y+100, 300);
	Animate_MoveDrawObject(manager->objects[2], manager->objects[2]->x, manager->objects[2]->y+100, 300);

}

void NewsManager_PushLeft(NewsManager *manager)
{

	manager->animation_id = Animate_MoveDrawObject(manager->objects[0], manager->objects[0]->x-1000, manager->objects[0]->y, 250);

}

void NewsManager_Animate(NewsManager *manager)
{

	switch (manager->news_state) {

		case PULL_RIGHT:
			NewsManager_PullRight(manager);
			break;
		case PUSH_DOWN:
			NewsManager_PushDown(manager);
			break;
		case PUSH_LEFT:
			NewsManager_PushLeft(manager);
			break;
	}

}

void NewsManager_Update(void *state)
{

	NewsManager *manager = state;
	NewsManager_CheckForNews();

	//if (manager->news_state == NOT_ANIMATING)
	//	return;

	if (!Animate_FinishedMoveAnimation(manager->animation_id))
		return;

	NewsManager_Transition(manager);
	NewsManager_Animate(manager);

}

void NewsManager_Add(void *state)
{

	NewsManager *manager = state;
	for (size_t i = 0; i < 3;i++)
		AddObjectToDrawLayer(manager->objects[i]);

}

void NewsManager_Delete(void *state)
{



}

Manager *NewsManager_Create(float x, float y)
{

	Manager *manager = Manager_Create();
	manager->Add     = NewsManager_Add;
	manager->Update  = NewsManager_Update;
	manager->Delete  = NewsManager_Delete;

	NewsManager *state  = malloc(sizeof(NewsManager));
	state->news_state   = NOT_ANIMATING;
	state->animation_id = 0;
	state->objects      = malloc(sizeof(DrawObject *) * 3);
	state->x_i          = x;
	state->y_i          = y;

	for (size_t i = 0; i < 3; i++) {

		state->objects[i]                   = Text_Create();
		state->objects[i]->x                = x;
		state->objects[i]->y                = y + 100*i;
		state->objects[i]->text.bitmap_path = "assets/images/all_buttons/button1.png";

	}

	manager->state = state;
	return manager;

}

Manager *NewsManager_Test() 
{

	Manager *manager = NewsManager_Create(900, 540);
	return manager;

}