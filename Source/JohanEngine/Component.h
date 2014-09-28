#ifndef PANEL_INCLUDE
#define PANEL_INCLUDE

#include <vector>

#include "models.h"
#include "float4.h"

enum ComponentType {
	ctBase,
	ctButton,
	ctLabel,
	ctWindow,
	ctEdit,
	ctBevel,
	ctDropdown,
};

class DLLIMPORT Component {
	public:
		Component(int left,int top,int width,int height);
		~Component();
		
		Component* parent;
		
		// Relative to parent
		int left;
		int top;
		
		// Relative to pixel [0,0]
		int absleft;
		int abstop;
		
		int width;
		int height;
		
		float4 backcolor;
		
		ComponentType type;
		
		// Handigheidjes
		void GetRect(RECT* result);
		
		// 3D representation
		Model* plane;
		void UpdatePlane(); // plane opnieuw laden
		void Move(int dx,int dy);
		void SetPos(int x,int y);
		void SetRect(int left,int top,int width,int height);
		void Resize(int newwidth,int newheight);
		
		// Child windows with position relatieve to us
		void AddChild(Component* child);
		std::vector<Component*> children;
		
    	// Nu kunnen we dingen doen als we tevoorschijn komen
    	void (*OnShow)(Component* Sender);
		
		// Visibility
		bool visible; // private maken?
		void Toggle();
		bool IsVisible();
		void Show(int mode);
};

#endif
