#ifndef CONTROLS_H
#define CONTROLS_H

void InitializeControls();
void HandleInput(ALLEGRO_EVENT event, ALLEGRO_EVENT_QUEUE *local_queue);

void HandlePauseMenu(ALLEGRO_EVENT local_event, ALLEGRO_EVENT_QUEUE *local_event_queue);

#endif