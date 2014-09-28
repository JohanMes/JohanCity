#ifndef SPOTLIGHT_INCLUDE
#define SPOTLIGHT_INCLUDE

#include <list>
#include "float3.h"
#include "float4x4.h"

class Lights;

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

class DLLIMPORT Spotlight {
	friend class Lights;
	
	float3 color;
	float3 dir;
	float3 pos;
	bool enabled;
	float angle;
	bool castshadows;
	
	// Matrices voor de renderer
	float4x4 matView;
	float4x4 matProj;
	float4x4 matViewProj;
	
	std::list<Spotlight*>::iterator bufferlocation;		
	public:
		Spotlight(float3 pos,float3 dir,float3 color,float angle,bool enabled);
		~Spotlight();

		float3 GetColor();
		float3 GetDirection();
		float3 GetPosition();
		float GetAngle();
		bool GetEnabled();
		bool GetCastShadows();
		void SetColor(float3 color);
		void SetDirection(float3 dir);
		void SetPosition(float3 pos);
		void SetAngle(float angle);
		void SetLookat(float3 pos);
		void SetEnabled(bool enabled);
		void SetCastShadows(bool castshadows);
		
		// Create simple perspective VP projection
		void GetProjection();
		
		void Print();
};

#endif
