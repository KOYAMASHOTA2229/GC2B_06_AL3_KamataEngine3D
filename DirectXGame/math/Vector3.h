#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	Vector3 operator+(const Vector3& num) const {

		Vector3 resultAdd;

		resultAdd.x = this->x + num.x;
		resultAdd.y = this->y + num.y;
		resultAdd.z = this->z + num.z;

		return resultAdd;
	}

	Vector3 operator*(float scalar) const {
		Vector3 resultMultiply;
		resultMultiply.x = this->x * scalar;
		resultMultiply.y = this->y * scalar;
		resultMultiply.z = this->z * scalar;
		return resultMultiply;
	}

	Vector3& operator+=(const Vector3& num) {
		x = this->x + num.x;
		y = this->y + num.y;
		z = this->z + num.z;
		return *this;
	}

	Vector3& operator-=(const Vector3& num) {
		x = this->x - num.x;
		y = this->y - num.y;
		z = this->z - num.z;
		return *this;
	}
};