#ifndef LABEL_INCLUDE
#define LABEL_INCLUDE

#include <string>

#include "Component.h"

class DLLIMPORT Label : public Component {
	protected:
		std::string caption;
	public:
		Label(int left,int top,int width,int height,const char* text,bool parentbg);
		~Label();
		
		// Captionspul
		void SetCaption(float value,const char* format);
		void SetCaption(const char* text);
		void AddCaption(const char* text);
		const char* GetCaption();
		
		// Handigheidje voor edits
		void AddChar(char key);
		void RemoveChar();
		
		// Rendering
		void DrawText(LPD3DXFONT font,LPD3DXSPRITE sprite);
};

#endif
