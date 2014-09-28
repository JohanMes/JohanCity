#ifndef SKYBOX_H
#define SKYBOX_H

#include "Object.h"
#include "Dirlight.h"

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

class DLLIMPORT Skybox {
	float skyboxradius;
	public:
		Skybox();
		~Skybox();
		
		Object* skybox; // sphere for now, creates one, calculates radius
		Object* sunobject; // puts sun plane on skybox at raduis R
		Dirlight* sunlight; // manages direction of this light
		
		void SetSkybox(const char* modelpath,const char* materialpath);
		void SetSunlight(const char* modelpath,const char* materialpath); // creates light too
		
		void Clear();
		void OnLostDevice();
		void OnResetDevice();
		void UpdateTime(float dt);
};

#endif
