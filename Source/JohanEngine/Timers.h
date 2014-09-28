#ifndef SCRIPTS_H
#define SCRIPTS_H

#include <list>
#include "Timer.h"

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

class DLLIMPORT Timers {
	std::list<Timer*> list;
	public:
		Timers();
		~Timers();
		
		void Add(Timer* timer);
		void Delete(Timer* timer);
		void Clear();
		void OnUpdateTime(float dt);
		void SaveToCSV();
};

#endif
