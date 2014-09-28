#ifndef OBJECT_INCLUDE
#define OBJECT_INCLUDE

#include <cfloat>
#include "Textures.h"
#include "Models.h"
#include "Material.h"
#include "float4x4.h"
#include "Animation.h"

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

class Object;

struct Collision {
	Object* object;
	float3 point;
	float t;
};

class DetailLevel {
	public:
		DetailLevel(Model* model,float maxdistance) {
			this->model = model;
			this->maxdistance = maxdistance;
		}
		Model* model;
		float maxdistance;
};

class DLLIMPORT Object {
	friend class Objects;
	
	char* name;
	Animation* animation; // remove when deleting object
	std::vector<DetailLevel*> detaillevels;

	std::list<Object*>::iterator bufferlocation;
	public:
	
		Object(const char* name);
		Object(const char* objectpath,const float3& pos,const float3& rot,float scale); // from .object file
		Object(const char* objectpath,const float3& pos,const float4x4& rot,float scale); // from .object file
		Object(const char* name,const char* modelpath,const char* materialpath,const float3& pos,const float3& rot,float scale); // from .obj + .mtl
		Object(const char* name,const char* modelpath,const char* materialpath,const float3& pos,const float4x4& rot,float scale); // from .obj + .mtl
		~Object();
		
		// TODO: getter van maken?
		Model* boundingmodel;
		
		// Materials
		Material* material; // TODO: aparte dump van maken?
		
		// Transformation
		float4x4 matRotation;
		float4x4 matTranslation;
		float4x4 matScaling; // store as float?
		float4x4 matWorld;
		float4x4 matWorldInverse; // don't recalculate
		
		// Transformed bounding model data
		float3 worldcenter;
		float worldr;
		
		// Engine-managed data
		bool visible;
		
		// Misc.
		bool castshadows;
		
		// Events
		void (*OnClick)(Object* Sender);
		
		void LoadFromFile(const char* objectpath);
		Collision IntersectSphere(float3 worldpos,float3 worlddir);
		Collision IntersectModel(float3 worldpos,float3 worlddir);
		const char* GetName();
		Model* GetBoundingModel();
		float3 GetTranslation();
		float3 GetScaling();
		void ClearDetailLevels();
		void AddDetailLevel(const char* modelpathmodel);
		void AddDetailLevel(Model* model);
		void AddDetailLevel(Model* model,float maxdistance);
		void AddDetailLevel(DetailLevel* detaillevel);
		DetailLevel* GetDetailLevel(float distance);
		void SetTranslation(const float3& pos);
		void SetTranslation(const float4x4& pos);
		void SetRotation(const float3& rotation);
		void SetRotationDeg(const float3& rotation);
		void SetRotation(const float4x4& rotation);
		void SetScaling(float scaling);
		void Move(const float3& dir);
		void SetName(const char* text);
		void SetAnimation(Animation* animation);
		void Print();
		void Update();
		void Clear();
};

bool CompareObject(Object* a,Object* b);

#endif
