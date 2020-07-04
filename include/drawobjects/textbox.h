#ifndef TEXTBOX_H
#define TEXTBOX_H

void InitTextBox(DrawObject *object);
void DrawTextBox(DrawObject *object);
void CleanUpTextBox(DrawObject *object);

DrawObject *CreateTextBoxObject(char *name, char *placeholder_text, short int limit_characters_to, unsigned short int flags);

#endif