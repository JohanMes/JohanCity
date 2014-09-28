#ifndef CAMERA_INCLUDE
#define CAMERA_INCLUDE

#include "Heightmap.h"
#include "FXVariable.h"
#include "Scene.h"

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

struct PLANE {
	float3 normal;
	float3 point;
	float d;
};

class DLLIMPORT Camera {
	
	FXVariable* FXCamerapos;
	FXVariable* FXMatProjInverse;
	
	void Update();
	void UpdateAngles();
	void UpdateVectors();
	void UpdateMatrices();
	void UpdateFrustumPlanes();
	void SetFrustumPlane(PLANE* curplane,float3& c1,float3& c2,float3& c3);
	
	public:
		Camera();
		~Camera();
		
		PLANE frustumplanes[6];
		float4x4 matView;
		float4x4 matViewInverse;
		float4x4 matProj;
		float4x4 matProjInverse;
		float4x4 matViewProj;
		float3 pos;
		float3 lookat;
		float3 dir; // normalized
		float3 up; // [0,1,0]^T
		float3 exactup; // precies naar 'boven'
		float3 right;
		float3 exactright; // cross van precies en dir	
		float angleH;
		float angleV;
		float ratio;
		float fov;
		float minviewdistance;
		float maxviewdistance;
		
		void SetPos(float3 pos);
		void SetLookAt(float3 lookat);
		void SetDir(float3 dir);
		void Move(float3 dir);
		void SetFOV(float fov);
		void SetRatio(float ratio);
		void SetMinViewDistance(float viewdistance);
		void SetMaxViewDistance(float viewdistance);
		void Print();
};

extern Camera* camera;

#endif
