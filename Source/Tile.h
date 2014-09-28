#ifndef TILE_H
#define TILE_H

#include "JohanEngine\Object.h"
#include "Batch.h"
#include "Building.h"

#include <vector>
using std::vector;

enum TileType {
	ttNone,
	ttGround,
	ttBuilding,
	ttResidential,
	ttCommercial,
	ttIndustrial,
	ttRoad, // delay = '1'
};

class Tile {
	void MapCoords(int x,int y);
	float3 GetCenter();
	public:
		Tile(int x,int y,float edgelen);
		~Tile();
		
		// Graphics related stuff
		int x;
		int y;
		float edgelen;
		Batch* batch; // batch this plane belongs too
		VERTEX* plane; // offset inside batch (+0 ... +3)
		
		// Game logic related stuff, only requisites are handled per tile
		TileType type;
		char typestring[32];
		bool powered;
		bool watered;
		bool connected;
		int checkid; // has been crawled by id for purpose X
		Building* building; // when non-NULL, above items are ignored and 
		
		// Caching of neighbors
		vector<Tile*> neighbors; // don't recreate this all the time
		int rneighbors; // radius covered by neighbor list
		
		void SetType(TileType type);
		void Bulldoze();
		void ShowInfo();
		void CheckConnection();
		void UpdateTime(float dt);
		std::vector<Tile*>& GetNeighbors(int r);
};

#endif
