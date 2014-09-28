#ifndef LIGHTS_INCLUDED
#define LIGHTS_INCLUDED

#include "Dirlight.h"
#include "Pointlight.h"
#include "Spotlight.h"
#include "FXVariable.h"

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

struct GPUDIRLIGHT {
	float3 direction;
	float3 color;
};

struct GPUPOINTLIGHT {
	float3 position;
	float3 color;
};

struct GPUSPOTLIGHT {
	float3 position;
	float3 direction;
	float3 color;
	float angle;
};

class DLLIMPORT Lights {
	
	// Vergeet de shaders niet bij te werken!
	static const int maxdirlights = 16;
	static const int maxpointlights = 16;
	static const int maxspotlights = 16;
	
	unsigned int updatecount;
	
	// GPU-format, updaten tijdens Update
	GPUDIRLIGHT gpudirlights[maxdirlights];
	GPUPOINTLIGHT gpupointlights[maxpointlights];
	GPUSPOTLIGHT gpuspotlights[maxspotlights];
	
	Dirlight* dirlightshadow;
	Pointlight* pointlightshadow;
	Spotlight* spotlightshadow;
	
	// GPU pointers
	FXVariable* FXDirlightshadowindex;
	FXVariable* FXNumdirlights;
	FXVariable* FXDirlights;
	FXVariable* FXPointlightshadowindex;
	FXVariable* FXNumpointlights;
	FXVariable* FXPointlights;
	FXVariable* FXSpotlightshadowindex;
	FXVariable* FXNumspotlights;
	FXVariable* FXSpotlights;
	FXVariable* FXFogcolor;
	FXVariable* FXAmbientLight; // skylight idea
	
	// Deze mag bij GPU-format
	friend class Renderer;
	public:
		
		Lights();
		~Lights();

		float ambientlight;
		std::list<Dirlight*> dirlights;
		std::list<Pointlight*> pointlights;
		std::list<Spotlight*> spotlights;
		
		// Vierkante schaduwtarget
		LPDIRECT3DTEXTURE9 ShadowTex;
		LPDIRECT3DSURFACE9 ShadowTexTopSurface;
		LPDIRECT3DSURFACE9 ShadowDepthTopSurface;
		bool castshadows; // true if there is a light that can cast shadows...
		
		// Matrices voor de renderer
		float4x4 matLightView;
		float4x4 matLightProj;
		float4x4 matLightViewProj;

		void SetAmbientlight(float ambient); // skylight
		Dirlight* AddDirlight(Dirlight* dirlight);
		Pointlight* AddPointlight(float3 pos,float3 color,bool enabled);
		Spotlight* AddSpotlight(Spotlight* spotlight);

		void DeleteDirlight(Dirlight* light);
		void DeletePointlight(Pointlight* light);
		void DeleteSpotlight(Spotlight* light);
		
		void GetProjection(float4x4* matView,float4x4* matProj,float4x4* matViewProj);
		
		void Clear();
		
		void Update(); // gpu-format bijwerken
		void BeginUpdate();
		void EndUpdate();
		
		void OnLostDevice();
		void OnResetDevice();
		void OnUpdateTime(float dt); // fit directional shadow casters to scene (sun moves, so recalculate)
		
		void Print();
};

#endif
