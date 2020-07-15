#ifndef PHYSICS_H
#define PHYSICS_H

#include <time.h>

typedef struct Position {

    float x;
    float y;

} Position;

typedef struct Velocity {

    float v_x;
    float v_y;

} Velocity;

typedef struct Acceleration {

    float a_x;
    float a_y;

} Acceleration;

typedef struct Kinematic {

    Position position;
    Velocity velocity;
    Acceleration acceleration;

} Kinematic;

Position ElapsedTime_GetNewPosition(Kinematic *kinematic, time_t t);
Position TimeDiff_GetDiffPosition(Kinematic *kinematic, time_t t, time_t dt);

float GetSpeed(float x, float n_x, long t);

#endif