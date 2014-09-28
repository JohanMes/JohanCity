#ifndef OBJECTS_INCLUDED
#define OBJECTS_INCLUDED

#include <list> // zodat we items kunnen weggooien terwijl bufferlocation geldig blijft

#include "Object.h"

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

class DLLIMPORT Objects {
	unsigned int updatecount;
	std::list<Object*> list;
	public:
		
		Objects();
		~Objects();
		
		Object* Add(Object* object);
		Object* AddPlane(const char* name,const char* matpath,const float3& pos,const float3& rot,float edgelen,unsigned int tiling,unsigned int textiling,Heightmap* height);
		
		Object* GetByName(const char* name);
		void Delete(Object* thisobject);
		void Clear();
		void Update(); // sorteren op shader
		
		void BeginUpdate();
		void EndUpdate();
		
		// Toegang tot lijst
		std::list<Object*>::iterator begin();
		std::list<Object*>::iterator end();
		unsigned int size();
		
		void Print();
};

#endif
