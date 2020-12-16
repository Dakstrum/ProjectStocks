#ifndef CONTROLS_H
#define CONTROLS_H

void InitializeControls();

void HandleInput(ALLEGRO_EVENT event);
void HandleMouseLocation();
void HandlePauseMenu(ALLEGRO_EVENT local_event);

int HoveringOverBitmap(DrawObject *bitmap);

#endif