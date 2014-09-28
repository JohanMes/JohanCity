#ifndef CLOCK_INCLUDE
#define CLOCK_INCLUDE

#include "Resource.h"

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

class DLLIMPORT Clock {
	
	__int64 starttime;
	float secpercount;
	
	public:
		
		Clock(bool start);
		~Clock();
		
		void Start();
		float GetTimeSec();
		float Reset();
};

#endif
