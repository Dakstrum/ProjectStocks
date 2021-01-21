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
void SwitchToRenderingNewsMenu();
void SwitchToRenderingAccountMenu();
void SwitchToRenderingPlayersMenu();

void HandleRendering(double dt);
void HandleWindowEvents(ALLEGRO_EVENT event);
void CleanUpRendering();
ALLEGRO_DISPLAY *GetDisplay();

#endif