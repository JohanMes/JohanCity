#include "JohanEngine\Scene.h"
#include "JohanEngine\Material.h"
#include "Batch.h"

Batch::Batch(int x,int y,int size,float edgelen) {
	
	this->size = size; // size along edge
	this->edgelen = edgelen;
	this->dirty = true; // send to gpu when tiles are done setting

	// Manually create object...
	plane = new Object("BatchPlane"); // create empty object
	plane->SetTranslation(float3(x,0/*(x + y) / 20.0f*/,y));
	plane->castshadows = false;
	
	// Assign it to our model
	plane->AddDetailLevel(GenerateBatchPlane());
	
	// And add a material
	plane->material->LoadFromFile("MasterTile.mtl");
	plane->Update();
}
Batch::~Batch() {
	delete plane;
}

Model* Batch::GenerateBatchPlane() {
	
	// 4 cornes for each square
	unsigned int numvertices = 4 * size * size;
	VERTEX* vb = new VERTEX[numvertices];
	
	// Walk each tile
	for(unsigned int i = 0;i < numvertices;i+=4) {
		unsigned int tilei = i / 4; // tile counter
		int x = tilei % size;
		int y = tilei / size; // 0 based
	
		vb[i + 0].pos = float3(x * edgelen,0,y * edgelen);
		vb[i + 1].pos = float3((x + 1) * edgelen,0,y * edgelen);
		vb[i + 2].pos = float3((x + 1) * edgelen,0,(y + 1) * edgelen);		
		vb[i + 3].pos = float3(x * edgelen,0,(y + 1) * edgelen);
		
		vb[i + 0].nor = float3(0,1,0);
		vb[i + 1].nor = float3(0,1,0);
		vb[i + 2].nor = float3(0,1,0);
		vb[i + 3].nor = float3(0,1,0);
		
		// Texture coords are set on first show
	}
	
	// 6 indices for each square (2 triangles)
	unsigned int numfaces = 2 * size * size;
	unsigned int* ib = new unsigned int[3*numfaces];
	
	// Walk each tile again
	for(unsigned int i = 0;i < 3*numfaces;i+=6) {
		unsigned int tilei = i / 6; // tile counter
		ib[i + 0] = tilei*4 + 0;
		ib[i + 1] = tilei*4 + 3;
		ib[i + 2] = tilei*4 + 1;
		ib[i + 3] = tilei*4 + 3;
		ib[i + 4] = tilei*4 + 2;
		ib[i + 5] = tilei*4 + 1;
	}
	
	Model* result = models->Add();
	result->LoadBuffer(vb,ib,numvertices,3*numfaces); // consumes buffers
	return result;
}
void Batch::Invalidate() {
	dirty = true;
}
void Batch::Validate() {
	if(dirty) {
		dirty = false;
		plane->GetDetailLevel(0)->model->SendToGPU();
	}
}
