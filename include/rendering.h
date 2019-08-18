#ifndef RENDERING_H
#define RENDERING_H

enum RenderingType 
{

    START_UP_SEQUENCE,
    MAIN_MENU

};

void *RenderingEntry(ALLEGRO_THREAD *thread, void *arg);
void SwitchToRenderingMainMenu();

#endif