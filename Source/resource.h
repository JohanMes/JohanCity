#ifndef RESOURCE_INCLUDED
#define RESOURCE_INCLUDED

#include "JohanEngine\Clock.h"
#include "JohanEngine\Window.h"
#include "JohanCity.h"
#include "Menu.h"

void ToggleFullscreen(void* data);
void ToggleOptions(void* data);
void ToggleConsole(void* data);

extern JohanCity* game;
extern Menu* menu;
extern POINT begmousedrag;
extern bool mousedragging;
extern Clock* updateclock;

#endif
