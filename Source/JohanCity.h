#ifndef JOHANCITY_H
#define JOHANCITY_H

#include <vector>
using std::vector;

#include <list>
using std::list;

#include "JohanEngine\Clock.h"
#include "Tile.h"
#include "Batch.h"
#include "Building.h"
#include "Menu.h"

class JohanCity {
	
	Clock* clock;
	
	// Call Resize() to perform a resize
	void ResizeGrid();
	void ResizeBatches();
	
	// Call UpdateTime to update the simulation
	void CheckBuildingRequisites();
	
	// Resend dirty batches to GPU
	void UpdateBuffers();
	
	// Optimized for specific power updating use
	void CheckPower(); // specialized optimized version, crawls from power plant outward...
	void ApplyPowerFrom(Tile* tile); // using recursive apply power from...
	
	// Optimized for specific water updating use
	void CheckWater();
	
	// TODO: optimize?
	void CheckConnection();

	public:
		JohanCity(int gridsize,float edgelen);
		~JohanCity();
		
		// Grid, array of buildings
		int gridsize;
		int batchsize; // size of GPU batches in tiles
		float edgelen; // length of edge of tile
		int checkid; // each tile crawl leaves this ID behind to prevent loops
		vector<vector<Tile*> > tiles; // each tile has its own plane and can spawn buildings
		vector<vector<Batch*> > batches; // for the gpu, divide grid in batchsize draw call batches
		list<Building*> buildings; // use a list, so we can efficiently remove buildings
		
		// Information about city
		int money;
		int population;
		int rdemand;
		int cdemand;
		int idemand;
		float checktilems;
		float checkbuildingms;
		float updatetilems;
		float updatebuildingms;
		char timingreport[256];
		
		// Set information about city
		void SetMoney(int value);
		void SetPopulation(int value);
		void SetR(int value);
		void SetC(int value);
		void SetI(int value);
		void AddR(int value);
		void AddC(int value);
		void AddI(int value);
		void UpdateTime(float dt);
		
		// Grid size modifiers
		void Resize(int gridsize,float edgelen);
		void Clear();
		
		// Grid access
		POINT RayToGridPos(float3 worldpos,float3 worlddir);
		Tile* WorldToTile(float3 worldpos);
		POINT WorldToGridPos(float3 worldpos); // world pos to indices
		float3 GridToWorldPos(POINT gridpos); // indices to world pos
		int LimitGrid(int value); // limit to valid grid pos
		void SortClampPoints(POINT* from,POINT* to);
		void Bulldoze(POINT from,POINT to); // fill with ground
		void BuildRoad(POINT from,POINT to,TileType type); // fill path instead of area
		void BuildZone(POINT from,POINT to,TileType type); // fill, add roads to selection
		void BuildType(POINT from,POINT to,MenuType type);
		void ShowInfo(POINT gridpos);
		
		// Simulation utilities
		Tile* FindTypeAround(Tile* tile,int r,TileType type);
		Tile* FindTypesAround(Tile* tile,int r,std::vector<TileType>& typelist);
		Tile* FindTypeNotBy(Tile* tile,int r,TileType notby,TileType type);
		Tile* FindTypeBy(Tile* tile,int r,TileType by,TileType type);
		Tile* FindTypesNotBy(Tile* tile,int r,std::vector<TileType>& notbylist,std::vector<TileType>& typelist);
		Tile* FindTypesBy(Tile* tile,int r,std::vector<TileType>& bylist,std::vector<TileType>& typelist);
		
		// Building stuff
		Building* AddBuilding(Building* building);
		void DeleteBuilding(Building* building);
};

#endif
