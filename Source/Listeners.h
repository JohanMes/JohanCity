#ifndef LISTENERS_INCLUDED
#define LISTENERS_INCLUDED

void ToggleFullscreen(void* data);
void ToggleOptions(void* data);
void ToggleConsole(void* data);
void MouseMove(WPARAM wParam,LPARAM lParam,int dx,int dy);
void MouseDown(WPARAM wParam,LPARAM lParam,bool handled);
void MouseUp(WPARAM wParam,LPARAM lParam,bool handled);
void MouseWheel(WPARAM wParam,LPARAM lParam);
void KeyDown(WPARAM wParam,LPARAM lParam,bool handled);
void UpdateTime(float dt);

#endif
