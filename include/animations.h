#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "drawobject.h"

void Animate_Initialize();
void Animate_MoveDrawObject(DrawObject *object, float n_x, float n_y, long animation_length);
void Animate_Update();

void Animate_DisableByLayer(unsigned char layer_index);

#endif