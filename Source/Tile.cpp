#include <vector>
#include "JohanEngine\Scene.h"
#include "Tile.h"
#include "Menu.h"
#include "resource.h"

Tile::Tile(int x,int y,float edgelen) {
	
	// Tile position
	this->x = x;
	this->y = y;
	this->edgelen = edgelen;
	
	// Graphics
	building = NULL;
	plane = NULL;
	
	// Logic
	type = ttNone;
	checkid = -1;
	powered = false;
	watered = false;
	connected = false;
}
Tile::~Tile() {
	Bulldoze();
}

void Tile::MapCoords(int x,int y) {
	
	// Plane not yet assigned, don't change 
	if(!plane) {
		return;
	}
	
	// Update when MasterTile changes
	int xtile = 4;
	int ytile = 4;

	// Map to MasterTile.dds
	float xstep = 1.0f/xtile;
	float ystep = 1.0f/ytile;
	float sigma = 0.03f;
	plane[0].tex = float2((x + 0) * xstep + sigma,(y + 0) * ystep + sigma);
	plane[1].tex = float2((x + 1) * xstep - sigma,(y + 0) * ystep + sigma);
	plane[2].tex = float2((x + 1) * xstep - sigma,(y + 1) * ystep - sigma);
	plane[3].tex = float2((x + 0) * xstep + sigma,(y + 1) * ystep - sigma);
	batch->dirty = true;
}
void Tile::SetType(TileType type) {
	if(type == this->type) {
		return;
	}
	
	// Don't let rezoning remove buildings
	if(building) {
		if(building->type == btResidential && type == ttResidential) {
			return;
		}
		if(building->type == btCommercial && type == ttCommercial) {
			return;
		}
		if(building->type == btIndustrial && type == ttIndustrial) {
			return;
		}
	}
	
	// Remove contents as type has changed
	Bulldoze();
		
	// Set type in int form
	this->type = type;
	
	// Set type in string form too
	switch(type) {
		case ttNone: {
			snprintf(typestring,32,"None");
			MapCoords(0,0); // ground too...
			break;
		}
		case ttGround: {
			snprintf(typestring,32,"Ground");
			MapCoords(0,0);
			break;
		}
		case ttResidential: {
			snprintf(typestring,32,"Residential 1");
			MapCoords(1,0);
			break;
		}
		case ttCommercial: {
			snprintf(typestring,32,"Commercial 1");
			MapCoords(2,0);
			break;
		}
		case ttIndustrial: {
			snprintf(typestring,32,"Industrial 1");
			MapCoords(3,0);
			break;
		}
		case ttBuilding: {
			snprintf(typestring,32,"Building");
			MapCoords(0,0);
			break;
		}
		case ttRoad: {
			snprintf(typestring,32,"Road");
			MapCoords(0,1);
			break;
		}
	}
}
float3 Tile::GetCenter() {
	return float3((x + 0.5) * edgelen,0,(y + 0.5) * edgelen);
}
void Tile::Bulldoze() {
	if(building) {
		game->DeleteBuilding(building); // handles pop changes, etc
		building = NULL;
	}
}
void Tile::ShowInfo() {
	if(building) {
		building->ShowInfo();
	} else {
		char text[512];
		switch(type) {
			case ttResidential:
			case ttCommercial:
			case ttIndustrial: {
				snprintf(text,512,
					"X = %d\r\nY = %d\r\nType = %s\r\nPowered = %d\r\nWatered = %d\r\nConnected = %d",
					x,
					y,
					typestring,
					powered,
					watered,
					connected);
				break;
			}
			default: {
				snprintf(text,512,
					"Type = %s",
					typestring);
			}
		}
		new Messagebox(text);
	}
}
void Tile::CheckConnection() {
	if(!building) {
		game->checkid++;
		connected = false;
		
		// 1) is there work within 2 tiles from us (walk distance)?
		std::vector<TileType> worklist;
		worklist.push_back(ttCommercial);
		worklist.push_back(ttIndustrial);
		Tile* walkwork = game->FindTypesAround(this,2,worklist);
		if(walkwork) {
			connected = true;
			return; // win
		}
		
		// 2) is there a means of transport nearby?
		std::vector<TileType> bylist;
		bylist.push_back(ttRoad);
		Tile* travelroad = game->FindTypesAround(this,2,bylist);
		if(travelroad) { // no means of travel within 2 tiles
			connected = true;
			return; // fail
		}
	}
}
void Tile::UpdateTime(float dt) {
	switch(type) {
		case ttResidential: {
			if(game->rdemand < 5) { 
				return;
			}
			
			// We must meet these requirements
			if(!powered || !watered || !connected) {
				return;
			}
			
			if(!building) { // not occupied
				game->AddBuilding(new Building(x,y,1,1,5,5,btResidential));
			}
			break;
		}
		case ttCommercial: {
			if(game->cdemand < 10) {
				return;
			}
			
			// We must meet these requirements
			if(!powered || !watered || !connected) {
				return;
			}
			
			if(!building) {
				game->AddBuilding(new Building(x,y,1,1,10,10,btCommercial));
			}
			break;
		}
		case ttIndustrial: {
			if(game->idemand < 10) {
				return;
			}
			
			// We must meet these requirements
			if(!powered || !watered || !connected) { // ignore water?
				return;
			}
			
			if(!building) {
				game->AddBuilding(new Building(x,y,1,1,10,10,btIndustrial));
			}
			break;
		}
	}
}
std::vector<Tile*>& Tile::GetNeighbors(int r) {
	
	// Don't recalculate
	if(r != rneighbors) {
	
		// Limit to edges of map
		int x1 = game->LimitGrid(x - r);
		int x2 = game->LimitGrid(x + r);
		int y1 = game->LimitGrid(y - r);
		int y2 = game->LimitGrid(y + r);
		
		neighbors.clear(); // capacity does not change
		neighbors.reserve((2*r+1)*(2*r+1)-1); // worst case
		for(int i = x1;i <= x2;i++) {
			for(int j = y1;j <= y2;j++) {
				if(!(i == x && j == y)) {
					neighbors.push_back(game->tiles[i][j]);
				}
			}
		}
		
		rneighbors = r;
	}
	return neighbors;
}
