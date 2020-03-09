#include"Geometry.h"
#include<DxLib.h>
#include<cmath>


void Rect::Draw() 
{
	DxLib::DrawBox(Left()*2, Top()*2, Right()*2, Bottom()*2, 0xffffffff, false);
}

void Rect::Draw(Vector2& offset) 
{
	DxLib::DrawBox((Left()+offset.x)*2, (Top()+offset.y)*2, (Right()+offset.x)*2, (Bottom()+offset.y)*2, 0xffffffff, false);
}

void Vector2::operator*=(float scale)
{
	x *= scale;
	y *= scale;
}

Vector2 Vector2::operator*(float scale) 
{
	return Vector2(x*scale, y*scale);
}

Vector2 operator+(const Vector2& va, const Vector2 vb) 
{
	return Vector2(va.x + vb.x, va.y + vb.y);
}

Vector2 operator-(const Vector2& va, const Vector2 vb)
{
	return Vector2(va.x - vb.x, va.y - vb.y);
}

float Vector2::Magnitude()const 
{
	return hypot(x, y);
}


void Vector2::Normalize()
{
	float mag = Magnitude();
	x /= mag;
	y /= mag;
}


Vector2 Vector2::Normalized() 
{
	float mag = Magnitude();
	return Vector2(x / mag,	y /mag);
}



float Dot(const Vector2& va, const Vector2& vb)
{
	return va.x*vb.x + va.y*vb.y;
}


float Cross(const Vector2& va, const Vector2& vb)
{
	return va.x*vb.y - vb.x*va.y;
}


float  operator*(const Vector2& va, const Vector2& vb)
{
	return Dot(va, vb);
}


float operator%(const Vector2& va, const Vector2& vb)
{
	return Cross(va, vb);
}

Color operator*(const Color  va, const Color vb)
{
	Color Return = Color(va.Red*vb.Red, va.Green*vb.Green, va.Blue*vb.Blue);
	Return.Clamp();
	return Return;
}


void  Vector2::operator+=(const Vector2& v)
{
	x += v.x;
	y += v.y;
}
void Vector2::operator-=(const Vector2& v)
{
	x -= v.x;
	y -= v.y;
}


void Vector3::operator*=(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
}

Vector3 Vector3::operator*(float scale)const 
{
	return Vector3(x*scale, y*scale,z*scale);
}

Vector3 operator+(const Vector3& va, const Vector3 vb) 
{
	return Vector3(va.x + vb.x, va.y + vb.y,va.z+vb.z);
}

Vector3 operator-(const Vector3& va, const Vector3 vb) 
{
	return Vector3(va.x - vb.x, va.y - vb.y,va.z-vb.z);
}

float Vector3::Magnitude()const 
{
	return sqrt(x*x+y*y+z*z);
}


void Vector3::Normalize() 
{
	float mag = Magnitude();
	x /= mag;
	y /= mag;
	z /= mag;
}


Vector3 Vector3::Normalized()
{
	float mag = Magnitude();
	return Vector3(x / mag, y / mag,z/mag);
}


float Dot(const Vector3& va, const Vector3& vb) 
{
	return va.x*vb.x + va.y*vb.y+va.z*vb.z;
}


Vector3 Cross(const Vector3& va, const Vector3& vb)
{
	return Vector3(va.z*vb.y-va.y*vb.z,va.z*vb.x-va.x*vb.z,va.x*vb.y - vb.x*va.y);
}


float operator*(const Vector3& va, const Vector3& vb)
{
	return Dot(va, vb);
}


Vector3 operator%(const Vector3& va, const Vector3& vb)
{
	return Cross(va, vb);
}


void Vector3::operator+=(const Vector3& v) 
{
	x += v.x;
	y += v.y;
	z += v.z;
}
void Vector3::operator-=(const Vector3& v) 
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}


void Color::ApplyDiffuse(float& Diffuse)
{
	Red *= (Diffuse);
	Green *= (Diffuse);
	Blue *= (Diffuse);
}

void Color::ApplySpecular(float& Specular)
{
	Red += Specular;
	Green += Specular;
	Blue += Specular;
}

void Color::AppplyMaterial(Material & Mat)
{
	ApplyDiffuse(Mat.DiffuseB);
	ApplySpecular(Mat.SpecularB);
	Clamp();
}

void Color::Clamp(const float Min, const float Max)
{
	Red = max(min(Red, Max), Min);
	Green = max(min(Green, Max), Min);
	Blue = max(min(Blue, Max), Min);
}
void Color::BlendColor(const Color vb,float Alpha)
{
	Red = Red * (1 - Alpha) + (vb.Red*Alpha);
	Blue = Blue * (1 - Alpha) + (vb.Blue*Alpha);
	Green = Green * (1 - Alpha) + (vb.Green*Alpha);
	Clamp();
}
Color Color::operator*(float scale) const
{
	Color ReturnCol;
	ReturnCol.Red = Red*scale;
	ReturnCol.Blue = Blue* scale;
	ReturnCol.Green = Green*scale;
	return ReturnCol;
}
