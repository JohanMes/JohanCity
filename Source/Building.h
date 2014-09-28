#ifndef BUILDING_H
#define BUILDING_H

#include "JohanEngine\Object.h"

#include <vector>
#include <list>

enum BuildingType {
	btNone,
	btResidential,
	btCommercial,
	btIndustrial,
	btPowerPlant,
	btPowerLine,
};

class Tile;

class Building {
	public:
		Building(int offset,int yoffset,int xwidth,int ywidth,int cur,int max,BuildingType type);
		~Building();
		
		// Location
		int xoffset;
		int yoffset;
		int xwidth; // in # tiles
		int ywidth; // idem
		BuildingType type;
		char typestring[32];
		
		// Logic
		bool powered;
		bool watered;
		bool connected;
		bool canreachwork;
		int maxpopulation; // max pop. for houses, jobs for commercial/industrial
		int curpopulation; // max cur. for houses, jobs for commercial/industrial
		std::vector<Tile*> neighbors;
			
		// Graphics
		Object* object;
		std::list<Building*>::iterator storage;
		
		void LoadObject(const char* modelpath,const char* materialpath);
		void LoadObject(const char* objectpath); // accounts for LOD too
		void SetPopulation(int cur,int max);
		void Build();
		void CacheNeighbors();
		void SetType(BuildingType type);
		float3 GetCorner();
		void ShowInfo();
		void CheckPower();
		void CheckWater();
		void CheckConnection();
		void FindWork();
		void UpdateTime(float dt);
};

#endif
