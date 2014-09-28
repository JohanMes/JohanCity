#ifndef OPTIONS_INCLUDE
#define OPTIONS_INCLUDE

#include "Components.h"

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

class DLLIMPORT COptionsDialog : public Window {
	
    static void OnShowDialog(Component* Sender);
    static void OnApplyClick(Component* Sender);
    public:
    	
    	COptionsDialog(int show);
    	~COptionsDialog();
    	
    	// Reso
    	Dropdown* cmbreso;
    	
    	// Schaduwdetail
    	Edit* edshadowmapsize;
    	
      	// Schaduwafstand
    	Edit* edshadowdistance;
    	
    	// Schaduwbias
    	Edit* edshadowbias;
			
    	// Objectdetail
    	Edit* edlodmultiplier;
    	
    	// SSAO-detail
    	Edit* edssaosamples;
    	
       	// SSAO-afstand
    	Edit* edssaodistance;
    	
    	// de rest van SSAO tweak je maar met de hand
    	
    	// Texturefilter
    	Dropdown* cmbfilter;
    	
    	// HDR
    	Edit* edexposure;
    	
    	// Knopjes
    	Button* btnapply;
		
		// Instellingenlezers
		void Load(); // onshow
		void Save(); // onclose?
};

extern COptionsDialog* OptionsDialog;

#endif
