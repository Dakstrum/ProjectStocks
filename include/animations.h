#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <stdbool.h>
#include <time.h>

#include "drawobject.h"

void Animate_Initialize();
unsigned int Animate_MoveDrawObject(DrawObject *object, float n_x, float n_y, long animation_length);
void Animate_Update(double dt);

bool Animate_FinishedMoveAnimation(unsigned int id);
void Animate_DisableByLayer(unsigned char layer_index);

#endif