#ifndef CURVE_INCLUDE
#define CURVE_INCLUDE

#include <d3dx9.h>
#include <vector>

#include "float3.h"

class Curve {
	
	// max order 30
	std::vector<float3> points;
	
	float length; // cached
	void Update(); // set length
	
	public:
		Curve();
		Curve(float3 p0,float3 p1);
		Curve(float3 p0,float3 p1,float3 p2);
		Curve(float3 p0,float3 p1,float3 p2,float3 p3);
		Curve(std::vector<float3>& points);
		~Curve();
		
		float3 GetPoint(float t);
		float3 GetTangent(float t);
		D3DXMATRIX GetAngle(float t);
		
		float3 GetStart();
		void SetEnd(float3 p0);
		
		float GetLength();
};

#endif
