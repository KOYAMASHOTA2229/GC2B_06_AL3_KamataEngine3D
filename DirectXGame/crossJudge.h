#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include "structAABB.h"

class crossJudge {

	public:

		static bool IsCollision(const AABB& aabb1, const AABB& aabb2);

};
