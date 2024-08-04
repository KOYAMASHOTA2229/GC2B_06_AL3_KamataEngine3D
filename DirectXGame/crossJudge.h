#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include "structAABB.h"

class crossJudge {

	public:

		static bool IsCollision(const AABB& aabb1, const AABB& aabb2);

		static Matrix4x4 MakeRotateZMatrix(float radian);

		static Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

};
