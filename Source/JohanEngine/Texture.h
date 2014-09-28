#ifndef TEXTURE_INCLUDE
#define TEXTURE_INCLUDE

#include <list>
#include <d3d9.h>

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

class DLLIMPORT Texture {
	friend class Textures;
	
	Texture(); // create empty shell
	Texture(const char* filename); // load from disk
	~Texture();
	
	std::list<Texture*>::iterator bufferlocation;
	public:
		LPDIRECT3DTEXTURE9 pointer;
		char* fullpath; // includes path
		char* filename; // filename only
		
		bool GetInfo(D3DSURFACE_DESC* result);
		int GetWidth();
		int GetHeight();
		__int64 GetSize();
		
		void Print();
};

#endif
