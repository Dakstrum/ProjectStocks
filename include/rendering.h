#ifndef RENDERING_H
#define RENDERING_H

enum RenderingType 
{

    START_UP_SEQUENCE,
    MAIN_MENU

};

void InitializeRendering();
void SwitchToRenderingMainMenu();
void SwitchToLoadingScreen();
void SwitchToRenderingStocksMenu();
void HandleRendering();
void HandleWindowEvents(ALLEGRO_EVENT event);
void CleanUpRendering();
ALLEGRO_DISPLAY *GetDisplay();
int refresh_rate;

#endif