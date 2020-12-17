#ifndef MAINMENU_H
#define MAINMENU_H

void MainMenu_Init();
void MainMenu_RenderLogic();

void MainMenu_Start_CB();
void MainMenu_Options_CB();
void MainMenu_Exit_CB();

void MainMenu_GitHub_CB();
void MainMenu_Twitter_CB();
void MainMenu_Youtube_CB();
void MainMenu_Website_CB();

int MainMenu_Active();

void CleanMainMenu();

#endif