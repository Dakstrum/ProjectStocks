#ifndef NEWSLIST_H
#define NEWSLIST_H

#include "drawobject.h"

DrawObject *NewsList_Create(float x, float y);
void NewsList_Insert(char *article);

#endif