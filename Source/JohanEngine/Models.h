#ifndef MODELS_INCLUDE
#define MODELS_INCLUDE

#include <stdio.h>
#include "Model.h"

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

class DLLIMPORT Models {
	std::list<Model*> list;
	public:
		Models();
		~Models();
		
		Model* Add(); // create empty shell
		Model* Add(const char* filename,bool sendtogpu); // add model from OBJ to pile
		Model* AddPlane(unsigned int tiling,unsigned int textiling,float edgelen,Heightmap* height,bool sendtogpu); // add plane to pile
		Model* Add(Model* model); // add custom one to pile
		Model* GetByFileName(const char* filename);
		Model* GetByFullPath(const char* fullpath);
		void Delete(Model* model);
		void Clear();
		void Print();
		void SaveToCSV();
};

extern Models* models;

#endif
