#include "crossJudge.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>

bool crossJudge::IsCollision(const AABB& aabb1, const AABB& aabb2) { 
	
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) && (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) && (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {
		// 戻り値をtrueに設定する
		return true;
	}
	return false; 
}

Matrix4x4 crossJudge::MakeRotateZMatrix(float radian) { 

	Matrix4x4 rotateZMatrix = {
		std::cos(radian), std::sin(radian), 0.0f, 0.0f, -std::sin(radian), std::cos(radian), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	return rotateZMatrix;

}

Vector3 crossJudge::Transform(const Vector3& vector, const Matrix4x4& matrix) { 
Vector3 result;

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}


