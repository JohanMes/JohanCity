#ifndef FXVARIABLE_INCLUDE
#define FXVARIABLE_INCLUDE

#include "FXHandle.h"
#include "textures.h"

class FXVariable : public FXHandle {
	public:
		FXVariable(const char* name);
		~FXVariable();
		
		// TODO: operators maken?
		void Set(void* data);
		void Set(float data);
		void Set(unsigned int data);
		
		void SetTexture(LPDIRECT3DBASETEXTURE9 texture);
		void SetTexture(Texture* texture);
};

#endif
