
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "newsmanager.h"
#include "animations.h"
#include "text.h"
#include "log.h"
#include "simulation_event.h"
#include "game.h"

#include "drawobject.h"
#include "drawlayers.h"
#include "window.h"

#include "audio.h"

typedef enum NewsManagerStates {

	NOT_ANIMATING,
	PULL_RIGHT,
	PUSH_DOWN,
	PUSH_LEFT

} NewsManagerStates;

typedef struct NewsManager {

	DrawObject **objects;
	NewsManagerStates news_state;
	unsigned int group_id;
	float x_i;
	float y_i;
	unsigned char active_boxes;
	unsigned char num_boxes;
	Vector *active_events;

} NewsManager;

static char *news_to_push = NULL;

uint32_t NewsManager_GetRandomSampleId()
{

	static char *sample_ids[5] = 
	{
		"female_uh_ha", 
		"female_hmm", 
		"male_hmm", 
		"male_hmm_2", 
		"male_hmm_3"
	};
	return audio_get_sample_id(sample_ids[rand()%5]);

}

void NewsManager_SetupPushLeft(NewsManager *manager)
{

	audio_play_sample(NewsManager_GetRandomSampleId());

	assert(news_to_push != NULL);
	DrawObject **objects = manager->objects;
	objects[0]->x = manager->x_i+1000;
	objects[0]->y = manager->y_i;
	SetTextContent(objects[0], news_to_push);

}

void NewsManager_PullRight(NewsManager *manager)
{

	manager->group_id = Animation_StartGroup();

	unsigned char last_box = manager->num_boxes - 1;
	Animate_MoveDrawObject(manager->objects[last_box], manager->objects[last_box]->x+1000, manager->objects[last_box]->y, 500);

	Animation_EndGroup();
}

void NewsManager_PushDown(NewsManager *manager)
{

	DrawObject **objects = manager->objects;

	for (size_t i = manager->active_boxes-1; i > 0;i--) {

		objects[i]->x = objects[i-1]->x;
		objects[i]->y = objects[i-1]->y;
		SetTextContent(objects[i], objects[i-1]->text.content);

	}

	NewsManager_SetupPushLeft(manager);

	manager->group_id = Animation_StartGroup();
	for (size_t i = 1; i < manager->active_boxes;i++) {

		Animate_MoveDrawObject(manager->objects[i], manager->objects[i]->x, manager->objects[i]->y+140, 300);

	}
	Animation_EndGroup();

}

void NewsManager_PushLeft(NewsManager *manager)
{

	manager->group_id = Animation_StartGroup();
	Animate_MoveDrawObject(manager->objects[0], manager->objects[0]->x-1000, manager->objects[0]->y, 250);
	Animation_EndGroup();

}

void NewsManager_PushNews(NewsManager *manager, char *content)
{

	news_to_push = content;
	if (manager->active_boxes == 0) {

		DrawObject **objects  = manager->objects;
		objects[0]->bit_flags |= SHOULD_BE_DRAWN;
		manager->news_state = PUSH_LEFT;
		manager->active_boxes++;

		SetTextContent(objects[0], content);

		NewsManager_SetupPushLeft(manager);
		NewsManager_PushLeft(manager);

	} else if (manager->active_boxes < manager->num_boxes) {

		DrawObject **objects = manager->objects;
		objects[manager->active_boxes]->bit_flags |= SHOULD_BE_DRAWN;

		manager->news_state = PUSH_DOWN;
		manager->active_boxes++;

		NewsManager_PushDown(manager);

	} else {

		manager->news_state = PULL_RIGHT;
		NewsManager_PullRight(manager);

	}

}

void NewsManager_CheckForNews(NewsManager *manager)
{

	Vector *events = Simulation_Event_GetLastEvents(1);
	SimulationEvent *events_temp = events->elements;
	SimulationEvent *active_events_temp = manager->active_events->elements;

	if (events->num_elements != 0) {

		bool found_match = false;
		for (size_t i = 0; i < manager->active_events->num_elements;i++) {

			if (active_events_temp[i].uid == events_temp[0].uid) {

				found_match = true;
				break;

			}

		}
		if (!found_match) {

			Vector_PushBack(manager->active_events, &events_temp[0]);
			NewsManager_PushNews(manager, ((SimulationEvent*)Vector_Last(manager->active_events))->event);

		}

	}
	Vector_Delete(events);

}

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
			manager->group_id   = 0;
			break;

	}

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
	if (manager->group_id == 0) {

		NewsManager_CheckForNews(manager);

	} else {

		if (!Animate_FinishedMoveGroupAnimation(manager->group_id))
			return;

		NewsManager_Transition(manager);
		NewsManager_Animate(manager);

	}


}

void NewsManager_Add(void *state)
{

	NewsManager *manager = state;
	for (size_t i = 0; i < manager->num_boxes;i++)
		AddObjectToDrawLayer(manager->objects[i]);

}

void NewsManager_Delete(void *state)
{

	NewsManager *manager = state;
	free(manager->objects);
	free(state);

}

Manager *NewsManager_Create(float x, float y)
{

	Manager *manager = Manager_Create();
	manager->Add     = NewsManager_Add;
	manager->Update  = NewsManager_Update;
	manager->Delete  = NewsManager_Delete;

	NewsManager *state  = malloc(sizeof(NewsManager));
	state->news_state   = NOT_ANIMATING;
	state->group_id     = 0;
	state->num_boxes    = 4;
	state->objects      = malloc(sizeof(DrawObject *) * state->num_boxes);
	state->x_i          = x;
	state->y_i          = y;
	state->active_boxes = 0;

	Vector *active_events = Simulation_Event_GetLastEvents(4);
	SimulationEvent *active_events_temp = active_events->elements;
	state->active_boxes = active_events->num_elements;
	state->active_events = active_events;

	for (size_t i = 0; i < state->num_boxes; i++) {

		state->objects[i]                   = Text_Create();
		state->objects[i]->width            = 350;
		state->objects[i]->x                = x;
		state->objects[i]->y                = y + 140 * i;
		state->objects[i]->text.bitmap_path = "assets/images/newsmenu/newsmenuassets/newsbox.png";

		if (state->active_boxes > i) {

			SetTextContent(state->objects[i], active_events_temp[i].event);
			state->objects[i]->bit_flags |= SHOULD_BE_DRAWN;

		} else {

			state->objects[i]->bit_flags ^= SHOULD_BE_DRAWN;

		}

	}

	manager->state = state;
	return manager;

}