#include "Plane.h"



Plane::Plane() :PlaneNormal(0, 0, 0), PlaneGridSize(30),GeometoryObject(0.9f)
{
};
Plane::Plane(float x, float y, float z, float d, int Size) :PlaneNormal(x, y, z), PlaneGridSize(Size),GeometoryObject(0.9f)
{
	Pos.y = d;
	PlaneNormal.Normalize();
};


Plane::~Plane()
{
}

std::pair<GeometoryObject::HitInfo, Color> Plane::IsHitRayAndObject(const Position3 & Eye, const Vector3 & Ray, const Vector3 Light, Vector3 & Normal, Position3 & HitPos)
{
	std::pair<HitInfo, Color> ReturnPair;

	ReturnPair.first.IsHit = CheckHit(Eye, Ray, Normal, HitPos);

	if (!ReturnPair.first.IsHit)
	{
		return ReturnPair;
	}

	ReturnPair.second = GetColor(Eye, Ray, HitPos);
	Vector3 Distance = (Eye - HitPos);
	ReturnPair.first.HitDistance = Distance.Magnitude();

	Normal = PlaneNormal;
	return ReturnPair;
}

bool Plane::CheckHit(const Position3 & eye, const Vector3 & ray, Vector3 & Normal, Position3 & HitPos)
{
	return (Dot(ray, PlaneNormal) < 0);
}

void Plane::AddPos(const Position3 & Add)
{
	Pos += Add;
}

Color Plane::GetColor(const Position3& EyePos, const Vector3& Ray, Position3& HitPos)
{
	Color ReturnColor(1, 0.8, 0.6f);

	float T = (Pos.y - Dot(EyePos, PlaneNormal)) / Dot(Ray, PlaneNormal);
	HitPos = EyePos + Ray * T;
	HitPos.x += Pos.x;
	HitPos.z += Pos.z;
	int ColorOffSet = 0;
	if (HitPos.x > 0)
	{
		ColorOffSet += 1;
	}
	if (HitPos.z > 0)
	{
		ColorOffSet += 1;
	}
	if (((int)HitPos.z / PlaneGridSize) % 2)
	{
		ColorOffSet += 1;
	}

	if (((int)(HitPos.x / PlaneGridSize) + ColorOffSet) % 2)
	{
		ReturnColor.Red = 0.5;
		ReturnColor.Green = 0.5;
		ReturnColor.Blue = 0.5;
	}


	return ReturnColor;
}