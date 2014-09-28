#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <list>
#include "Animation.h"

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

class DLLIMPORT Animations {
	std::list<Animation*> list;
	public:
		Animations();
		~Animations();
		
		Animation* Add(Animation* animation);
		Animation* Add(Object* object,AnimationType type);
		void Delete(Animation* animation);
		void Clear();
		void OnUpdateTime(float dt);
		void Print();
		void SaveToCSV();
};

#endif
