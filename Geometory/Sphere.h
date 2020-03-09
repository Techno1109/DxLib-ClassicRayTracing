#pragma once
#include "GeometoryObject.h"
class Sphere :
	public GeometoryObject
{
public:
	Sphere();
	Sphere(float r, Position3& p, float red, float green, float blue, float Reflect);
	~Sphere();

	std::pair<HitInfo, Color> IsHitRayAndObject(const Position3& Eye, const Vector3& Ray, const Vector3 Light, Vector3& Normal, Position3& HitPos) override;
	bool CheckHit(const Position3& eye, const Vector3& ray, Vector3& Normal, Position3& HitPos) override;
	void AddPos(const Position3& Add) override;
private:
	float Radius;//”¼Œa
	Color SphereColor;
	Material GetDrawMaterial(const Position3& Eye, const Vector3& Ray,const Vector3& Light, Vector3& Normal);
};

