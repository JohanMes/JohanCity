#ifndef HEIGHT_INCLUDE
#define HEIGHT_INCLUDE

#include "resource.h"

class Heightmap {
	public:
		Heightmap();
		Heightmap(const char* path,float minheight,float maxheight,float widthmeters);
		~Heightmap();
		
		int widthpixels;
		float widthmeters;
		float minheight;
		float maxheight;
		
		float** colortable;
		
		void Load(const char* path,float minheight,float maxheight);
		float GetNearestHeight(float x,float z);
		float GetBilinearHeight(float x,float z);
};

#endif
