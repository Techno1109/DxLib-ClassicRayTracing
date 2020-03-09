#pragma once
#include "Geometry.h"
#include <Utility>

class GeometoryObject
{
public:
	struct HitInfo
	{
		bool IsHit;
		float HitDistance;
	};

	GeometoryObject(float Reflect);
	virtual ~GeometoryObject();

	virtual std::pair<HitInfo,Color> IsHitRayAndObject(const Position3& Eye, const Vector3& Ray, const Vector3 Light, Vector3& Normal, Position3& HitPos)=0;
	virtual bool CheckHit(const Position3& eye, const Vector3& ray, Vector3& Normal, Position3& HitPos)=0;
	virtual void AddPos(const Position3& Add)=0;
	const Position3& GetPos();
	float& GetReflectance();
protected:
	float Reflectance=0;
	Position3 Pos;
};

