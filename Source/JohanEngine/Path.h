#ifndef PATH_INCLUDE
#define PATH_INCLUDE

#include <d3dx9.h>
#include <vector>

#include "Curve.h"
#include "float4x4.h"

class Path {
	
	std::vector<Curve*> list;
	
	public:
		Path();
		Path(Curve* piece);
		~Path();
		
		void Add(Curve* piece);
		void AddToTail(Curve* piece);
		
		Curve* GetCurve(float t);
		float3 GetPoint(float t);
		float3 GetTangent(float t);
		float4x4 GetAngle(float t);
		
		float GetLength();
};

#endif
