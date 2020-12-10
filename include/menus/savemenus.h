#ifndef SAVEMENUS_H
#define SAVEMENUS_H

void LoadSaveMenu_Init();

void LoadSaveMenu_NewSave_CB();
void LoadSaveMenu_Back_CB();
void LoadSaveMenu_LoadSave_CB();
void LoadSaveMenu_DeleteSave_CB();

void NewSave_BCB();
void NewSaveMenu_Back_CB();
void NewSaveMenu_CreateSave_CB();

void SaveMenus_Clean();

#endif