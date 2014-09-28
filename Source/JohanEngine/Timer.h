#ifndef TIMER_H
#define TIMER_H

#include <list>

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

class DLLIMPORT Timer {
	friend class Timers;
	std::list<Timer*>::iterator bufferlocation;
	public:
		Timer();
		Timer(int ms,bool set,void (*OnTimer)(void* data));
		~Timer();

		bool set; // opposite of paused
		int ms;
		void (*OnTimer)(void* data);

		bool GetFinished();
		void Set(int ms,bool set);
		void OnUpdateTime(float dt);
};

#endif
