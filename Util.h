#pragma once
#include "Geometry.h"
#include<math.h>
#include<Windows.h>

namespace
{
	float Clamp(float Param, const float Min = 0.0f, const float Max = 1.0f)
	{
		return  max(min(Param, Max), Min);
	}

	Vector3 GetReflectVector(const Vector3& InVec, const Vector3& NormalVec)
	{
		return InVec - NormalVec * (2 * Dot(NormalVec, InVec));
	}
}