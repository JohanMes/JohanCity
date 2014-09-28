#ifndef INTERFACE_H
#define INTERFACE_H

#include <vector>

#include "Components.h"
#include "Key.h"

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

class DLLIMPORT Interface {
	
	std::vector<Component*> componentlist;
	std::vector<Key*> keylist;
	Component* hotcomponent; // highlightkleurtjes enzo
	Component* dragcomponent; // verplaatswindow
	Component* downcomponent; // welke knop is atm geklikt?
	Component* focuscomponent; // welke edit is nu bezig?
	friend class Renderer; // mag bij list
	
	// Intercept messages here (and send them to the "On" ones)
	void DoKeyChar(WPARAM wParam,LPARAM lParam);
	void DoKeyDown(WPARAM wParam,LPARAM lParam);
	void DoKeyUp(WPARAM wParam,LPARAM lParam);
	void DoMouseWheel(WPARAM wParam,LPARAM lParam);
	void DoMouseMove(WPARAM wParam,LPARAM lParam);
	void DoMouseDown(WPARAM wParam,LPARAM lParam);
	void DoMouseUp(WPARAM wParam,LPARAM lParam);
	
	public:
		Interface();
		~Interface();
		
		// Muisstate
		POINT mousepos; // int vector
		bool lmousedown; // TODO: Key van maken?
		bool mmousedown;
		bool rmousedown;
		
		void (*OnKeyChar)(WPARAM wParam,LPARAM lParam,bool handled);
		void (*OnKeyDown)(WPARAM wParam,LPARAM lParam,bool handled);
		void (*OnKeyUp)(WPARAM wParam,LPARAM lParam,bool handled);
		void (*OnMouseWheel)(WPARAM wParam,LPARAM lParam);
		void (*OnMouseMove)(WPARAM wParam,LPARAM lParam,int dx,int dy);
		void (*OnMouseDown)(WPARAM wParam,LPARAM lParam,bool handled);
		void (*OnMouseUp)(WPARAM wParam,LPARAM lParam,bool handled);
		
		// Hiermee berichten van Windows verwerken
		bool OnMessage(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam);
        
    	// Componentjes...
		Component* GetComponentAt(int left,int top);
		void AddComponent(Component* thiscomponent);
		void RemoveComponent(Component* thiscomponent);
		
		// Knopjes
		void AddKey(Key* thiskey);
		void RemoveKey(Key* thiskey);
		
		// Etc
		void OnLostDevice();
		void OnResetDevice(); // update ui on resize
};

extern Interface* ui;

#endif
