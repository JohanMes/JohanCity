#ifndef POINTLIGHT_INCLUDE
#define POINTLIGHT_INCLUDE

#include <list>
#include "float3.h"

class Lights;

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

class DLLIMPORT Pointlight {
	friend class Lights;
	
	Pointlight(Lights* parent,float3 pos,float3 color,bool enabled);
	~Pointlight();
	
	float3 color;
	float3 pos;
	bool enabled;
	bool castshadows; // always false for now
	Lights* factory;
	
	std::list<Pointlight*>::iterator bufferlocation;
	public:
		float3 GetColor();
		float3 GetPosition();
		bool GetEnabled();
//		bool GetCastShadows();
		void SetColor(float3 color);
		void SetPosition(float3 pos);
		void SetEnabled(bool enabled);
//		void SetCastShadows(bool castshadows);
};

#endif
