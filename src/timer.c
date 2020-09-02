
#include <assert.h>
#include <time.h>

#include "timer.h"
#include "shared.h"

static struct timespec last_render_update;
static double previous_time = 0.0;

static bool paused = false;

void Timer_Init() 
{

	last_render_update = GetCurrentTime();

}

double Timer_GetDiff() 
{

	if (paused)
		return 0.0;

	struct timespec current_time = GetCurrentTime();
	double dt = GetDoubleMilliDiff(&current_time, &last_render_update);
	last_render_update = current_time;

	return dt;

}

void Timer_Pause()
{
	assert(!paused);

	previous_time = Timer_GetDiff();
	paused = true;

}

void Timer_Unpause()
{
	assert(paused);

	paused = false;
	last_render_update = GetOffsetTime(-previous_time);
	previous_time = 0.0;

}