#ifndef TEXTURES_INCLUDE
#define TEXTURES_INCLUDE

#include <list>
#include "Texture.h"

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

class DLLIMPORT Textures {
	std::list<Texture*> list;
	public:
		Textures();
		~Textures();
		
		Texture* Add(); // add empty shell
		Texture* Add(const char* filename); // perform dupe check
		Texture* Add(Texture* texture); // blindly add
		Texture* GetByFullPath(const char* fullpath);
		Texture* GetByFileName(const char* filename);
		void Delete(Texture* thistexture);
		void Clear();
		void Print();
		void SaveToCSV();
};

extern Textures* textures;

#endif
