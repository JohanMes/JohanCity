#ifndef OPTIONS_H
#define OPTIONS_H

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

class DLLIMPORT Options {
	public:
		Options(const char* inifile);
		~Options();
	
		// Reso
		int backbufferwidth;
		int backbufferheight;
		int refreshrate;
		bool fullscreen;
		bool maxfsresolution;
		
		// Schaduwen
		int shadowmapsize; // 0 == disable
		float shadowdistance;
		float shadowbias;
		
		// Objects
		float lodmultiplier;
		float minviewdistance; // not tweakable
		float maxviewdistance; // idem
	
		// SSAO
		int ssaosamples; // 0 == disable
		float ssaodistance;
		float ssaorad;
		float ssaomulti;
		float ssaoscale;
		float ssaobias;
		float ssaodepth;
		
		// AA
		int aasamples; // TODO: remove?
		int aaquality; // TODO: remove?
		
		// Textures
		char texturefilter[128]; // random guess
		int afsamples;
		
		// HDR
		bool enablehdr;
		float exposure;
		float bloomrampstart;
		float bloomrampend;
		float bloommultiplier;
		
		// Misc.
		bool enablevsync;
		bool usesoftware;
		
		void SetToDefaults();
		void LoadFromFile(const char* inipath);
		void FindMaxResolution();
};

extern Options* options;

#endif
