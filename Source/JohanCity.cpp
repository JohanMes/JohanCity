#include <algorithm>
#include "JohanEngine\Clock.h"
#include "JohanEngine\Renderer.h"
#include "JohanCity.h"
#include "Menu.h"
#include "resource.h"

JohanCity::JohanCity(int gridsize,float edgelen) {
	
	// Resize grid
	Resize(gridsize,edgelen);
	
	// Misc.
	checkid = 0;
	timingreport[0] = 0;
	clock = new Clock(false);
	
	// Set default user values
	SetPopulation(0);
	SetMoney(50000);
	SetR(50); // beginpop
	SetC(10); // 1 kantoortje?
	SetI(70);
}
JohanCity::~JohanCity() {
	delete clock;
	Clear();
}

void JohanCity::Clear() {
	
	// Remove grid
	for(int i = 0;i < tiles.size();i++) {
		for(int j = 0;j < tiles[i].size();j++) {
			delete tiles[i][j];
		}
	}
	gridsize = 0;

	// Remove batches
	for(int i = 0;i < batches.size();i++) {
		for(int j = 0;j < batches[i].size();j++) {
			delete batches[i][j];
		}
	}
	batchsize = 0;
	
	// Remove buildings
	for(std::list<Building*>::iterator i = buildings.begin();i != buildings.end();i++) {
		delete *i;
	}
}
void JohanCity::UpdateBuffers() {
	
	// Send plane batches to GPU when done
	for(int i = 0;i < batches.size();i++) {
		for(int j = 0;j < batches[i].size();j++) {
			if(batches[i][j]->dirty) {
				batches[i][j]->plane->GetDetailLevel(0)->model->SendToGPU();
				batches[i][j]->dirty = false;
			}
		}
	}
}
void JohanCity::SetMoney(int value) {
	money = value;
	menu->SetMoney(money);
}
void JohanCity::SetPopulation(int value) {
	population = value;
	menu->SetPopulation(population);	
}
void JohanCity::SetR(int value) {
	rdemand = value;
	menu->SetR(rdemand);
}
void JohanCity::SetC(int value) {
	cdemand = value;
	menu->SetC(cdemand);
}
void JohanCity::SetI(int value) {
	idemand = value;
	menu->SetI(idemand);
}
void JohanCity::AddR(int value) {
	SetR(rdemand + value);
}
void JohanCity::AddC(int value) {
	SetC(cdemand + value);
}
void JohanCity::AddI(int value) {
	SetI(idemand + value);
}
void JohanCity::UpdateTime(float dt) {

	clock->Start();
	
	// Optimized version of power checking...
	CheckPower(); // crawls from power plant outward...
	
	// Optimized version of water checking...
	CheckWater(); // crawls from water source outward...
	
	// TODO: optimize?
	CheckConnection();

	checktilems = clock->Reset() * 1000.0f;
	
	// Then apply tile info to buildings
	CheckBuildingRequisites();
	
	checkbuildingms = clock->Reset() * 1000.0f;

	// Done checking for required items? Spend the RCI
	for(int i = 0;i < tiles.size();i++) {
		for(int j = 0;j < tiles[i].size();j++) {
			TileType type = tiles[i][j]->type;
			if(type == ttResidential || type == ttCommercial || type == ttIndustrial) {
				tiles[i][j]->UpdateTime(dt);
			}
			
			// Undo checks (so they won't get invalid)?
			tiles[i][j]->checkid = -1;
//			tiles[i][j]->powered = false;
//			tiles[i][j]->watered = false;
//			tiles[i][j]->connected = false;
		}
	}
	
	updatetilems = clock->Reset() * 1000.0f;
	
	for(std::list<Building*>::iterator i = buildings.begin();i != buildings.end();i++) {
		Building* building = *i;
		building->UpdateTime(dt);
	}
	
	updatebuildingms = clock->Reset() * 1000.0f;
	
	snprintf(timingreport,256,
		"Check tile time: %.2f ms\r\nCheck building time: %.2f ms\r\nUpdate tile time: %.2f ms\r\nUpdate building time: %.2f ms\r\n",
		checktilems,
		checkbuildingms,
		updatetilems,
		updatebuildingms);
	
	UpdateBuffers();
}
void JohanCity::SortClampPoints(POINT* from,POINT* to) {
	
	// Sort X
	int x1 = from->x;
	int x2 = to->x;
	if(x2 < x1) {
		int tmp = x1;
		x1 = x2;
		x2 = tmp;
	}
	
	// Sort Y
	int y1 = from->y;
	int y2 = to->y;
	if(y2 < y1) {
		int tmp = y1;
		y1 = y2;
		y2 = tmp;
	}
	
	// Limit to edges of map
	x1 = LimitGrid(x1);
	x2 = LimitGrid(x2);
	y1 = LimitGrid(y1);
	y2 = LimitGrid(y2);
	
	from->x = x1;
	from->y = y1;
	
	to->x = x2;
	to->y = y2;
}
void JohanCity::Bulldoze(POINT from,POINT to) {
	SortClampPoints(&from,&to);
	for(int x = from.x;x <= to.x;x++) {
		for(int y = from.y;y <= to.y;y++) {
			tiles[x][y]->SetType(ttGround);	
			tiles[x][y]->Bulldoze();
		}
	}
	UpdateBuffers();
}
void JohanCity::BuildRoad(POINT from,POINT to,TileType type) {
	SortClampPoints(&from,&to);
	for(int x = from.x;x <= to.x;x++) { // TODO: bocht toevoegen?
		for(int y = from.y;y <= to.y;y++) {
			tiles[x][y]->SetType(ttRoad);	
		}
	}
	UpdateBuffers();
}
void JohanCity::BuildZone(POINT from,POINT to,TileType type) {
	SortClampPoints(&from,&to);
	switch(type) {
		case ttResidential:
		case ttCommercial:
		case ttIndustrial: { 
			if(to.x - from.x > 3 && to.y - from.y > 3) {// add roads when size is more than 4x4
				for(int x = from.x;x <= to.x;x++) {
					for(int y = from.y;y <= to.y;y++) {
						if((x - from.x) % 5 == 0 || (y - from.y) % 5 == 0) {
							tiles[x][y]->SetType(ttRoad);
						} else {
							tiles[x][y]->SetType(type);	
						}
					}
				}
			} else {
				for(int x = from.x;x <= to.x;x++) {
					for(int y = from.y;y <= to.y;y++) {
						tiles[x][y]->SetType(type);	
					}
				}
			}
			break;
		}
	}
	UpdateBuffers();
}
void JohanCity::BuildType(POINT from,POINT to,MenuType type) {
	switch(type) {
		case mtResidential: {
			BuildZone(from,to,ttResidential);
			break;
		}
		case mtCommercial: {
			BuildZone(from,to,ttCommercial);
			break;
		}
		case mtIndustrial: {
			BuildZone(from,to,ttIndustrial);
			break;
		}
		case mtRoad: {
			BuildRoad(from,to,ttRoad);
			break;
		}
		case mtBulldoze: {
			Bulldoze(from,to);
			break;
		}
		case mtPowerPlant: {
			AddBuilding(new Building(from.x,from.y,2,2,0,0,btPowerPlant));
			break;
		}
		case mtPowerLine: {
			AddBuilding(new Building(from.x,from.y,1,1,0,0,btPowerLine));
			break;
		}
	}
	UpdateBuffers();
}
int JohanCity::LimitGrid(int value) {
	return std::min(gridsize-1,std::max(value,0));
}
void JohanCity::ResizeGrid() {
	
	// Resize grid
	tiles.resize(gridsize);
	for(int i = 0;i < this->gridsize;i++) {
		tiles[i].resize(gridsize);
	}
	
	// Create new tiles
	for(int i = 0;i < tiles.size();i++) {
		for(int j = 0;j < tiles[i].size();j++) {
			tiles[i][j] = new Tile(i,j,edgelen);
		}
	}
}
void JohanCity::ResizeBatches() {
	
	// Resize batches
	int numbatches = gridsize/batchsize;
	batches.resize(numbatches);
	for(int i = 0;i < batches.size();i++) {
		batches[i].resize(numbatches);
	}
	
	// Create batches
	for(int i = 0;i < batches.size();i++) {
		for(int j = 0;j < batches[i].size();j++) {
			batches[i][j] = new Batch(batchsize * edgelen * i,batchsize * edgelen * j,batchsize,edgelen); // Create a custom plane which does not reuse vertices...
		}
	}
}
void JohanCity::Resize(int gridsize,float edgelen) {
	
	// nicely remove old tiles and batches
	Clear();
	
	// Change parameters
	this->gridsize = gridsize;
	this->edgelen = edgelen;
	this->batchsize = gridsize/8;

	// Create new grid and recreate batches
	ResizeGrid();
	ResizeBatches(); // creates smaller piles of tiles to reduce draw calls
	
	// Look exactly at the center of our grid
	float3 focus = float3(gridsize*edgelen/2-300,0,gridsize*edgelen/2-300);
	camera->SetPos(focus + float3(-500,700,-500));
	camera->SetLookAt(focus);
	
	// Assign batches and objects to tiles
	for(int i = 0;i < tiles.size();i++) {
		for(int j = 0;j < tiles[i].size();j++) {
			
			// Determine where in the current batch we are
			int tilei = i % batchsize;
			int tilej = j % batchsize;
			
			Batch* batch = batches[i / batchsize][j / batchsize]; // batch this tile belongs to
			Model* plane = batch->plane->GetDetailLevel(0)->model; // model this tile is owner of
			tiles[i][j]->batch = batch;
			
			// Determine vertex index in batch
			int index = 4 * tilei + 4 * batchsize * tilej;
			tiles[i][j]->plane = &plane->localvertexbuffer[index]; // offset inside batch
		}
	}
	
	// Set default tile type
	for(int i = 0;i < tiles.size();i++) {
		for(int j = 0;j < tiles[i].size();j++) {
			tiles[i][j]->SetType(ttGround);
		}
	}
	
	UpdateBuffers(); // send all batches to the GPU
}
void JohanCity::CheckBuildingRequisites() {
	for(std::list<Building*>::iterator i = buildings.begin();i != buildings.end();i++) {
		Building* building = *i;
		BuildingType type = building->type;
		if(type == btResidential || type == btCommercial || type == btIndustrial) {
			building->CheckPower();
			building->CheckWater();
			building->CheckConnection();
			
			if(type == btResidential) {
				building->FindWork();
			}
		}
	}
}
POINT JohanCity::RayToGridPos(float3 worldpos,float3 worlddir) {
	for(int i = 0;i < batches.size();i++) {
		for(int j = 0;j < batches.size();j++) {
			Object* ground = batches[i][j]->plane;
			Collision batchtest = ground->IntersectModel(worldpos,worlddir);
			if(batchtest.object) { // we hit a plane!
				return WorldToGridPos(batchtest.point);
			}
		}
	}
	return POINT();
}
Tile* JohanCity::WorldToTile(float3 worldpos) {
	POINT index = WorldToGridPos(worldpos);
	if(index.x >= 0 && index.x < gridsize && index.y >= 0 && index.y < gridsize) {
		return tiles[index.x][index.y];
	} else {
		return NULL;
	}
}
POINT JohanCity::WorldToGridPos(float3 worldpos) {
	
	POINT result;
	
	// determine x and y/z *indices*
	result.x = floor(worldpos.x / edgelen);
	result.y = floor(worldpos.z / edgelen);
	return result;
}
float3 JohanCity::GridToWorldPos(POINT gridpos) {
	
	float3 result;
	
	// Return unrounded indices...
	result.x = edgelen * gridpos.x;
	result.y = 0; // TODO, add height difference
	result.z = edgelen * gridpos.y;
	return result;
}
void JohanCity::CheckPower() {
	checkid++;
	
	// Undo previous checks...
	for(int i = 0;i < tiles.size();i++) {
		for(int j = 0;j < tiles[i].size();j++) {
			tiles[i][j]->powered = false;
		}
	}
	
	for(std::list<Building*>::iterator i = buildings.begin();i != buildings.end();i++) {
		Building* building = *i;
		if(building->type == btPowerPlant) {
			
			// Crawl outward in all directions
			ApplyPowerFrom(tiles[building->xoffset][building->yoffset]);
		}
	}
}
void JohanCity::ApplyPowerFrom(Tile* tile) {
	
	// Don't bother repeating checks of this tile
	tile->checkid = checkid;
	tile->powered = true; // apply without checking
	
	// Get list of (max 8) neighboring tiles
	std::vector<Tile*> neighbors = tile->GetNeighbors(1);
	
	// Check each tile for connecting roads
	for(unsigned int i = 0;i < neighbors.size();i++) {
		Tile* neighbor = neighbors[i];
		
		// Did we already check that tile?
		if(neighbor->checkid != checkid) {
			
			// check new path if we are allowed to travel by it
			if(neighbor->type != ttGround) {
				
				// Spawn a crawler in this tile
				ApplyPowerFrom(neighbor);
			}
		}
	}	
}
void JohanCity::CheckWater() {
	for(int i = 0;i < tiles.size();i++) {
		for(int j = 0;j < tiles[i].size();j++) {
			TileType type = tiles[i][j]->type;
			
			// Only zoning areas need to check power, connection and water
			if(type == ttResidential || type == ttCommercial || type == ttIndustrial) {
				tiles[i][j]->watered = true;
			}
		}
	}	
}
void JohanCity::CheckConnection() {
	for(int i = 0;i < tiles.size();i++) {
		for(int j = 0;j < tiles[i].size();j++) {
			TileType type = tiles[i][j]->type;
			
			// Only zoning areas need to check power, connection and water
			if(type == ttResidential || type == ttCommercial || type == ttIndustrial) {
				tiles[i][j]->CheckConnection();
			}
		}
	}
}
Tile* JohanCity::FindTypeAround(Tile* tile,int r,TileType type) {
	
	// Get list of neighbors
	std::vector<Tile*> list = tile->GetNeighbors(r);
	
	// Is there a tile of type type in it?
	for(unsigned int i = 0;i < list.size();i++) {
		if(list[i]->type == type) {
			return list[i];
		}		
	}
	return NULL;
}
Tile* JohanCity::FindTypesAround(Tile* tile,int r,std::vector<TileType>& typelist) {
	
	// Get list of neighbors
	std::vector<Tile*> list = tile->GetNeighbors(r);
	
	// Is there a tile of type type in it?
	for(unsigned int i = 0;i < list.size();i++) {
		if(std::find(typelist.begin(),typelist.end(),list[i]->type) != typelist.end()) {
			return list[i];
		}
	}
	return NULL;
}
Tile* JohanCity::FindTypeNotBy(Tile* tile,int r,TileType notby,TileType type) {
	
	// Don't bother repeating checks of this tile
	tile->checkid = checkid;
	
	// Can it be found around us?
	Tile* reachtile = FindTypeAround(tile,r,type);
	if(reachtile) { // return first in list
		return reachtile;
	}
	
	// Get list of (max 8) neighboring tiles
	std::vector<Tile*> neighbors = tile->GetNeighbors(r);
	
	// Check each tile for connecting roads
	for(unsigned int i = 0;i < neighbors.size();i++) {
		Tile* neighbor = neighbors[i];
		
		// Did we already check that tile?
		if(neighbor->checkid != checkid) {
			
			// check new path if we are allowed to travel by it
			if(neighbor->type != notby) {
				
				// Spawn a crawler in that tile
				Tile* result = FindTypeNotBy(neighbor,r,notby,type);
				if(result) { // found work using this path? done
					return result;
				}
			}
		}
	}
	
	return NULL;
}
Tile* JohanCity::FindTypeBy(Tile* tile,int r,TileType by,TileType type) {
	
	// Don't bother repeating checks of this tile
	tile->checkid = checkid;
	
	// Can it be found around us?
	Tile* reachtile = FindTypeAround(tile,r,type);
	if(reachtile) { // return first in list
		return reachtile;
	}
	
	// Get list of neighboring tiles
	std::vector<Tile*> neighbors = tile->GetNeighbors(r);

	// Check each tile for connecting roads
	for(unsigned int i = 0;i < neighbors.size();i++) {
		Tile* neighbor = neighbors[i];
		
		// Did we already check that tile?
		if(neighbor->checkid != checkid) {
			
			// check new path if we are allowed to travel by it
			if(neighbor->type == by) {
				
				// Spawn a crawler in that tile
				Tile* result = FindTypeNotBy(neighbor,r,by,type);
				if(result) { // found work using this path? done
					return result;
				}
			}
		}
	}
	
	return NULL;
}
Tile* JohanCity::FindTypesNotBy(Tile* tile,int r,std::vector<TileType>& notbylist,std::vector<TileType>& typelist) {
	
	// Don't bother repeating checks of this tile
	tile->checkid = checkid;
	
	// Can it be found around us?
	Tile* reachtile = FindTypesAround(tile,r,typelist);
	if(reachtile) { // return first in list
		return reachtile;
	}
	
	// Get list of neighboring tiles
	std::vector<Tile*> neighbors = tile->GetNeighbors(r);
	
	// Check each tile for connecting roads
	for(unsigned int i = 0;i < neighbors.size();i++) {
		Tile* neighbor = neighbors[i];
		
		// Did we already check that tile?
		if(neighbor->checkid != checkid) {
			
			// check new path if we are allowed to travel by it
			if(std::find(notbylist.begin(),notbylist.end(),neighbor->type) == notbylist.end()) {
				
				// Spawn a crawler in that tile
				Tile* result = FindTypesNotBy(neighbor,r,notbylist,typelist);
				if(result) { // found work using this path? done
					return result;
				}
			}
		}
	}
	
	return NULL;
}
Tile* JohanCity::FindTypesBy(Tile* tile,int r,std::vector<TileType>& bylist,std::vector<TileType>& typelist) {
	
	// Don't bother repeating checks of this tile
	tile->checkid = checkid;
	
	// Can it be found around us?
	Tile* reachtile = FindTypesAround(tile,r,typelist);
	if(reachtile) { // return first in list
		return reachtile;
	}
	
	// Get list of neighboring tiles
	std::vector<Tile*> neighbors = tile->GetNeighbors(r);
	
	// Check each tile for connecting roads
	for(unsigned int i = 0;i < neighbors.size();i++) {
		Tile* neighbor = neighbors[i];
		
		// Did we already check that tile?
		if(neighbor->checkid != checkid) {
			
			// check new path if we are allowed to travel by it
			if(std::find(bylist.begin(),bylist.end(),neighbor->type) != bylist.end()) {
				
				// Spawn a crawler in that tile
				Tile* result = FindTypesBy(neighbor,r,bylist,typelist);
				if(result) { // found work using this path? done
					return result;
				}
			}
		}
	}
	
	return NULL;
}
void JohanCity::ShowInfo(POINT gridpos) {
	tiles[gridpos.x][gridpos.y]->ShowInfo();
}
Building* JohanCity::AddBuilding(Building* building) {
	buildings.push_back(building);
	building->storage = --buildings.end();
	return building;
}
void JohanCity::DeleteBuilding(Building* building) {
	buildings.erase(building->storage);	
	delete building;
}
