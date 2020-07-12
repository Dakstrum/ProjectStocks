
#include "physics.h"

float GetNewPosition(float pos, float v, float a, time_t time_elapsed)
{

    return 0.0;

}

void SetNewKinematicPosition(Kinematic *kinematic, time_t time_elapsed) 
{

    kinematic->position.x = GetNewPosition(kinematic->position.x, kinematic->velocity.v_x, kinematic->acceleration.a_x, time_elapsed);
    kinematic->position.y = GetNewPosition(kinematic->position.y, kinematic->velocity.v_y, kinematic->acceleration.a_y, time_elapsed);

}