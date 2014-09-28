#ifndef BATCH_H
#define BATCH_H

#include "JohanEngine\Object.h"

class Batch {
	Model* GenerateBatchPlane();
	public:
		Batch(int x,int y,int size,float edgelen);
		~Batch();
		
		Object* plane;
		bool dirty; // false -> SendToGPU
		int size; // size along one edge
		float edgelen; // edgelen of single tile
		
		void Invalidate();
		void Validate();
};

#endif
