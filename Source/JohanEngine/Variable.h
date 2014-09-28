#ifndef VARIABLE_INCLUDE
#define VARIABLE_INCLUDE

#include "Object.h"
#include "Path.h"
#include "Clock.h"
#include "Timer.h"

enum VarKind {
	vtInt,
	vtFloat,
	vtString,
	vtObject,
	vtPath, // bezier access
	vtFloat3,
	vtTimer,
	vtUnknown,
};

class Var {
	void SetType(const char* type);
	public:
		Var(VarKind type,const char* name,int level);
		Var(const char* type,const char* name,int level);
		~Var();
		
		int GetIntValue();
		float GetFloatValue();
		char* GetStringValue();
		float3* GetFloat3Value();
		
		bool Equals(Var* rvalue);
		bool Greater(Var* rvalue);
		bool Less(Var* rvalue);
		
		char* name;
		VarKind type;
		int level;
		union {
			int intvalue;
			float floatvalue;
			char* stringvalue;
			Object* objectvalue;
			Path* pathvalue;
			float3* float3value;
			Timer* timervalue;
		};
};

#endif
