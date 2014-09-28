#ifndef STATE_INCLUDE
#define STATE_INCLUDE

#include "resource.h"

class State {
	D3DRENDERSTATETYPE RenderState;
	DWORD Value;
	public:
		State(D3DRENDERSTATETYPE NewState);
		~State();
		
		void Set(DWORD NewValue);
};

#endif
