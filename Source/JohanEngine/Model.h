#ifndef MODEL_INCLUDE
#define MODEL_INCLUDE

#include <list>

#include "Heightmap.h"
#include "resource.h"

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

struct VERTEX {
	float3 pos;
	float2 tex;
	float3 nor;
	float3 tan;
	float3 bin;
};

class DLLIMPORT Model {
	friend class Models; // manages creation and destruction
	
	Model(); // create empty shell
	Model(const char* filename,bool sendtogpu); // load OBJ
	~Model();
	
	std::list<Model*>::iterator bufferlocation;
	public:

		// Pointers naar VRAM
		LPDIRECT3DVERTEXBUFFER9 vertexbuffer;
		LPDIRECT3DINDEXBUFFER9 indexbuffer;
		
		// Sphere approximation
		float r;
		float3 center;
		
		// Kopietje voor de CPU
		VERTEX* localvertexbuffer;
		unsigned int* localindexbuffer;

		unsigned int numvertices;
		unsigned int numfaces;
		
		// En de naam nog
		char* fullpath;
		char* filename;
		
		__int64 GetSize();
		void FreeBuffers();
		void CreateTangents();
		void Optimize();
		void SendToGPU();
		void GetBoundingSphere();
		void LoadFromOBJ(const char* filename);
		void Load2DQuad(float left,float top,float right,float bottom);
		void LoadPlane(unsigned int tiling,unsigned int textiling,float edgelen,Heightmap* height);
		void LoadBuffer(VERTEX* vb,unsigned int* ib,unsigned int numvertices,unsigned int numindices);
		void LoadParticle();
		
		void Print();
};

#endif
