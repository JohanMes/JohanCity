#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <list>
#include "float3.h"

class Object;

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

enum AnimationType {
	atLinear,
	atLinearLoop,
	// ease
	// bezier
};

class Keyframe {
	public:
		Keyframe(float t,float3 position) {
			this->t = t;
			this->position = position;
		}
		float t;
		float3 position;
};

class DLLIMPORT Animation {
	friend class Animations;

	std::list<Animation*>::iterator bufferlocation;
	public:
		Animation(Object* object,AnimationType type);
		~Animation();
	
		AnimationType type;
		Object* object;
		float t;
		std::vector<Keyframe*> keyframes;
		void (*OnAnimationEnd)(void* data);
		
		bool GetFinished();
		float GetPeriod();
		void OnUpdateTime(float dt);
		int GetKeyframeBefore(float t);
		int GetKeyframeAfter(float t);
		void AddKeyframe(float t,float3 position);
		void Print();
};

#endif
