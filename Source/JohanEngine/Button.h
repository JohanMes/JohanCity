#ifndef BUTTON_INCLUDE
#define BUTTON_INCLUDE

#include "Label.h"
#include "resource.h"

class DLLIMPORT Button : public Label {
	public:
		Button(int left,int top,int width,int height,const char* text);
		~Button();
		
		float4 downcolor; // click
		float4 hotcolor; // hover
		bool down;
		bool hot;
		
		void (*OnClick)(Component* Sender);
		void DrawText(LPD3DXFONT font,LPD3DXSPRITE sprite);
};

#endif
