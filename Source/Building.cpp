#include "JohanEngine\Scene.h"
#include "Tile.h"
#include "Building.h"
#include "resource.h"

Building::Building(int xoffset,int yoffset,int xwidth,int ywidth,int cur,int max,BuildingType type) {
	
	// Set position info (0 based)
	this->xoffset = xoffset;
	this->yoffset = yoffset;
	this->xwidth = xwidth;
	this->ywidth = ywidth;
	
	// Bulldoze occupied tiles...
	for(int x = xoffset;x < xoffset + xwidth;x++) {
		for(int y = yoffset;y < yoffset + ywidth;y++) {
			game->tiles[x][y]->Bulldoze(); // remove buildings
			game->tiles[x][y]->SetType(ttBuilding); // TODO: create building 'underlay'
		}
	}
	
	// Assign self to bulldozed tiles...
	for(int x = xoffset;x < xoffset + xwidth;x++) {
		for(int y = yoffset;y < yoffset + ywidth;y++) {
			game->tiles[x][y]->building = this;
		}
	}

	// Set type for own tile
	this->type = btNone;
	SetType(type);
	
	// Set population (should be 0 for power lines etc)
	this->curpopulation = cur;
	this->maxpopulation = max;

	// Create a list of neighbors
	CacheNeighbors();
	
	// Put 3D object in world
	Build();
}
Building::~Building() {
	
	// Gooi 3D-prut weg
	if(object) {
		delete object;
	}
	
	if(type == btResidential) {// Gooi pop weg als we het over huizen hebben
		game->SetPopulation(game->population - curpopulation);
	} else if(type == btCommercial) { // Pump C back into the system
		game->SetC(game->cdemand + curpopulation);
	} else if(type == btIndustrial) { // Pump I back into the system
		game->SetI(game->idemand + curpopulation);
	}
	
	// Unassign building...
	for(int x = xoffset;x < xoffset + xwidth;x++) {
		for(int y = yoffset;y < yoffset + ywidth;y++) {
			game->tiles[x][y]->building = NULL;
		}
	}
	
	// Set ground type to ground...
	for(int x = xoffset;x < xoffset + xwidth;x++) {
		for(int y = yoffset;y < yoffset + ywidth;y++) {
			game->tiles[x][y]->SetType(ttGround);
		}
	}
}
void Building::SetType(BuildingType type) {
	if(this->type == type) {
		return; // don't waste cycles
	}
	
	this->type = type;
	
	switch(type) {
		case btNone: {
			snprintf(typestring,32,"None");
			break;
		}
		case btPowerLine: {
			snprintf(typestring,32,"Power Line");
			break;
		}
		case btPowerPlant: {
			snprintf(typestring,32,"Power Plant");
			break;
		}
		case btResidential: {
			snprintf(typestring,32,"Residential 1");
			break;
		}
		case btCommercial: {
			snprintf(typestring,32,"Commercial 1");
			break;
		}
		case btIndustrial: {
			snprintf(typestring,32,"Industrial 1");
			break;
		}
	}
}
void Building::Build() {
	switch(type) {
		case btResidential: {
			game->SetPopulation(game->population + curpopulation);
			game->AddR(-curpopulation);
			game->AddC(curpopulation * 0.2); // add 0.2 commercial jobs per pop
			game->AddI(curpopulation * 1); // add 1 industrial ...
			
			// Load random for now
			float random = RandomRange(0,3);
			if(random < 1) {
				LoadObject("Residential\\Residential1.obj","White.mtl");
			} else if(random < 2) {
				LoadObject("Residential\\Residential2.obj","White.mtl");
			} else {
				LoadObject("Residential\\Residential3.obj","White.mtl");
			}
			break;
		}
		case btCommercial: {
			game->AddR(curpopulation * 3.5); // add 3 demand per job (not only for work)
			game->AddC(-curpopulation);
			LoadObject("Commercial\\Commercial1.obj","White.mtl");
			break;
		}
		case btIndustrial: {
			game->AddR(curpopulation * 0.7); // jobs create population growth
			game->AddC(curpopulation * 0.1);
			game->AddI(-curpopulation);
			LoadObject("Industrial\\Industrial1.obj","White.mtl");
			break;
		}
		case btPowerPlant: {
			LoadObject("Buildings\\PowerPlant.obj","White.mtl");
			break;
		}
		case btPowerLine: {
			LoadObject("Buildings\\PowerLine.obj","White.mtl");
			break;
		}
	}
}
float3 Building::GetCorner() {
	return float3(xoffset,0,yoffset) * game->edgelen;
}
void Building::LoadObject(const char* modelpath,const char* materialpath) {
	object = new Object("Prop",modelpath,materialpath,GetCorner(),0,1);
}
void Building::LoadObject(const char* objectpath) {
	object = new Object(objectpath,GetCorner(),0,1);
}
void Building::SetPopulation(int cur,int max) {
	curpopulation = cur;
	maxpopulation = max;
}
void Building::ShowInfo() {
	char text[512];
	switch(type) {
		case btResidential: {
			snprintf(text,512,
				"X = %d\r\nY = %d\r\nType = %s\r\nPowered = %d\r\nWatered = %d\r\nConnected = %d\r\nHas work = %d\r\nPopulation = %d/%d",
				xoffset,
				yoffset,
				typestring,
				powered,
				watered,
				connected,
				canreachwork,
				curpopulation,
				maxpopulation);
			break;
		}
		case btCommercial:
		case btIndustrial: {
			snprintf(text,512,
				"X = %d\r\nY = %d\r\nType = %s\r\nPowered = %d\r\nWatered = %d\r\nConnected = %d\r\nJobs = %d/%d",
				xoffset,
				yoffset,
				typestring,
				powered,
				watered,
				connected,
				curpopulation,
				maxpopulation);
			break;
		}
		default: {
			snprintf(text,32,"Type = %s",typestring);
			break;
		}
	}
	new Messagebox(text);
}
void Building::CacheNeighbors() {
	neighbors.reserve(2*xwidth + 2*ywidth + 4);
	
	// left side, incl. corners
	if(xoffset > 0) {
		for(int y = game->LimitGrid(yoffset-1);y < game->LimitGrid(yoffset + ywidth);y++) {
			neighbors.push_back(game->tiles[xoffset-1][y]);
		}
	}
	
	// right side, incl. corners
	if(xoffset < game->gridsize-1) {
		for(int y = game->LimitGrid(yoffset-1);y < game->LimitGrid(yoffset + ywidth);y++) {
			neighbors.push_back(game->tiles[xoffset+1][y]);
		}
	}
	
	// bottom, excl. corners
	if(yoffset > 0) {
		for(int x = game->LimitGrid(xoffset);x < game->LimitGrid(xoffset + xwidth - 1);x++) {
			neighbors.push_back(game->tiles[x][yoffset-1]);
		}
	}
	
	// top, excl. corners
	if(yoffset < game->gridsize-1) {
		for(int x = game->LimitGrid(xoffset);x < game->LimitGrid(xoffset + xwidth - 1);x++) {
			neighbors.push_back(game->tiles[x][yoffset+1]);
		}
	}
}
void Building::CheckPower() {
	
	powered = false;
	
	// Simply check if adjacent tiles/buildings have power...
	for(int i = 0;i < neighbors.size();i++) {
		if(neighbors[i]->powered) {
			powered = true;
			break;
		}
	}
}
void Building::CheckWater() {
	
	watered = false;
	
	// Simply check if adjacent tiles/buildings have power...
	for(int i = 0;i < neighbors.size();i++) {
		if(neighbors[i]->powered) {
			watered = true;
			break;
		}
	}
}
void Building::CheckConnection() {
	
	connected = false;
	
	// Simply check if adjacent tiles/buildings have power...
	for(int i = 0;i < neighbors.size();i++) {
		if(neighbors[i]->type == ttRoad) { // only roads are accepted for now
			connected = true;
			break;
		}
	}
}

void Building::FindWork() {
	game->checkid++;
	canreachwork = true; // pessimistic default
	
	// Not connected means no work to go to...
	if(!connected) {
		return;
	}
	
//	// 1) is there work within 2 tiles from us (walk distance)?
//	std::vector<TileType> worklist;
//	worklist.push_back(ttCommercial);
//	worklist.push_back(ttIndustrial);
//	Tile* walkwork = game->FindTypesAround(this,2,worklist);
//	if(walkwork) {
//		canreachwork = true;
//		return; // win
//	}
//
//	// 2) is there some place we can start traveling with?
//	std::vector<TileType> bylist;
//	bylist.push_back(ttRoad);
//	Tile* travelroad = game->FindTypesAround(this,2,bylist);
//	if(!travelroad) { // no means of travel within 2 tiles
//		return; // fail
//	}
//	
//	// 3) does it lead to work within 2 tiles?
//	Tile* travelwork = game->FindTypesBy(this,2,bylist,worklist);
//	if(travelwork) {
//		canreachwork = true;
//	}
}
void Building::UpdateTime(float dt) {
	// iets...
}
