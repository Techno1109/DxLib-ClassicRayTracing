#include "Sphere.h"
#include "Util.h"


Sphere::Sphere() :Radius(0), SphereColor(0, 0, 0),GeometoryObject(0)
{
}

Sphere::Sphere(float r, Position3& p, float red, float green, float blue, float Reflect) : Radius(r), SphereColor(red, green, blue), GeometoryObject(Reflect)
{
	Pos = p;
}

Sphere::~Sphere()
{
}

std::pair<GeometoryObject::HitInfo, Color> Sphere::IsHitRayAndObject(const Position3& Eye, const Vector3& Ray, const Vector3 Light, Vector3& Normal, Position3& HitPos)
{
	std::pair<HitInfo, Color> ReturnPair;

	ReturnPair.first.IsHit =CheckHit(Eye,Ray,Normal,HitPos);

	if (!ReturnPair.first.IsHit)
	{
		return ReturnPair;
	}

	Vector3 Distance = (Eye - HitPos);
	ReturnPair.first.HitDistance = Distance.Magnitude();

	auto ThisMat = GetDrawMaterial(Eye, Ray,Light ,Normal);
	auto& Col = ReturnPair.second;
	Col = SphereColor;
	Col.AppplyMaterial(ThisMat);

	return ReturnPair;
}

bool Sphere::CheckHit(const Position3 & eye, const Vector3 & ray, Vector3 & Normal, Position3 & HitPos)
{
	Vector3 ToSphereVec = Pos - eye;

	float DotRay = Dot(ToSphereVec, ray);
	if (DotRay<0)
	{
		return false;
	}

	//円の中心と垂線として考えられる点
	Position3 ToCenterPos = ray * DotRay;

	Vector3 ToCenterVec = ToSphereVec - ToCenterPos;

	float Tmp = ToCenterVec.Magnitude();

	if (Radius >= Tmp)
	{
		//Wの長さ
		float Wlength = (Radius*Radius - Tmp * Tmp);
		Wlength = sqrt(Wlength);

		//交点(当たった場所)
		HitPos = eye + ray * (DotRay - Wlength);

		//法線
		Normal = Pos - HitPos;
		//正規化する
		Normal.Normalize();
		return true;
	}

	return false;
}

void Sphere::AddPos(const Position3 & Add)
{
	Pos += Add;
}

Material Sphere::GetDrawMaterial(const Position3& Eye, const Vector3& Ray, const Vector3& Light, Vector3& Normal)
{
	Material ReturnMat;

	//ライト方向
	Vector3 UseLight = Light;
	UseLight.Normalize();

	ReturnMat.DiffuseB = Dot(Normal, UseLight);
	ReturnMat.DiffuseB = Clamp(ReturnMat.DiffuseB);

	ReturnMat.SpecularB = 0;

	Vector3 RLight = GetReflectVector(UseLight, Normal);
	RLight.Normalize();
	if (Dot(RLight, Normal) <= 0)
	{
		Vector3 rEye = Ray * -1;
		rEye.Normalize();
		ReturnMat.SpecularB = Clamp(Dot(RLight, rEye));
		ReturnMat.SpecularB = pow(ReturnMat.SpecularB, 30);
	}
	return  ReturnMat;
}
