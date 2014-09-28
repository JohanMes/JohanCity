#ifndef DROPDOWN_INCLUDE
#define DROPDOWN_INCLUDE

#include "Component.h"

#include <string>

class DLLIMPORT Dropdown : public Component {
	
	// En hier mag je het mee doen
	std::vector<std::string> items;
		
	public:
		Dropdown(int left,int top,int width,int height);
		~Dropdown();
		
		int itemheight;
		
		// Dropdowns
		bool dropped;
		int selindex;
		
		// Collapse or not to collapse
		void Toggle();
		
		// Adding items...
		void AddItem(const char* text);
		
		// Getting items
		const char* GetItem(unsigned int index);
		const char* GetSelection();
		unsigned int GetItemCount();
	
		// Dingen selecteren
		void SetSelection(const char* text);
		
		// Rendering
		void DrawText(LPD3DXFONT font,LPD3DXSPRITE sprite);
};

#endif
