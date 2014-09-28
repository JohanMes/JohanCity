#ifndef WINDOW_INCLUDE
#define WINDOW_INCLUDE

#include "resource.h"
#include "Label.h"

class DLLIMPORT Window : public Label {
	public:
		Window(int left,int top,int width,int height,const char* text);
		~Window();
		
		bool dragging;
		
		// Rendering
		void DrawText(LPD3DXFONT font,LPD3DXSPRITE sprite);
};

#endif
