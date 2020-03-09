#include "GeometoryObject.h"



GeometoryObject::GeometoryObject(float Reflect)
{
	Reflectance = Reflect;
	Pos =Position3(0, 0, 0);
}


GeometoryObject::~GeometoryObject()
{
}

const Position3 & GeometoryObject::GetPos()
{
	return Pos;
}

float& GeometoryObject::GetReflectance()
{
	return Reflectance;
}
