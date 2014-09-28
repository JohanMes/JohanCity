#ifndef CONSOLE_INCLUDE
#define CONSOLE_INCLUDE

#include "Components.h"
#include "Clock.h"
#include "float4x4.h"

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

class DLLIMPORT CConsoleDialog : public Window {

	std::vector<Clock*> clocks;
	
    static void Save(Component* Sender);
    static void GetInput(Component* Sender);

    public:
    	
    	CConsoleDialog(int show);
    	~CConsoleDialog();
    	
		char writebuf[500];
       	void WriteVar(const char* name,int value);
       	void WriteVar(const char* name,unsigned int value);
       	void WriteVar(const char* name,void* value);
		void WriteVar(const char* name,bool value);
		void WriteVar(const char* name,float value);
		void WriteVar(const char* name,const float2& value);
		void WriteVar(const char* name,const float3& value);
		void WriteVar(const char* name,const float4& value);
		void WriteVar(const char* name,const float4x4& value);
		void WriteVar(const char* name,const char* value);
		
		void Write(const char* format,...);
		
		void WriteBeginTimer(const char* text);
		void WriteEndTimer(const char* text);

		// 3D window
		Label* textctrl;
		Button* savebtn;
		Edit* inputctrl;
		
       	// Opties
		void GetInput();
		void ProcessString(const char *input);
};

extern CConsoleDialog* ConsoleDialog;

#endif
