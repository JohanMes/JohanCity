#ifndef MSGBOX_INCLUDE
#define MSGBOX_INCLUDE

#include "Components.h"

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

class DLLIMPORT Messagebox : public Window {
	
	static void OKClick(Component* Sender);

    public:
    	
    	Label* lbltext;
    	Button* okbtn;
    	
    	Messagebox(const char* text);
    	~Messagebox();
    	
    	void AddToInterface();
};

#endif
