#pragma once
#include "GeometoryObject.h"
class Plane :
	public GeometoryObject
{
public:
	Plane();
	Plane(float x, float y, float z, float d, int Size);
	~Plane();
	std::pair<HitInfo, Color> IsHitRayAndObject(const Position3& Eye, const Vector3& Ray, const Vector3 Light,  Vector3& Normal, Position3& HitPos)override;
	bool CheckHit(const Position3& eye, const Vector3& ray, Vector3& Normal, Position3& HitPos) override;
	void AddPos(const Position3& Add) override;
private:
	Vector3 PlaneNormal;
	int PlaneGridSize;

	Color Plane::GetColor(const Position3& EyePos, const Vector3& Ray, Position3& HitPos);
};

