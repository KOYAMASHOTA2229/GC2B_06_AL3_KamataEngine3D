#pragma once
#include <cmath>
#include <iostream>
#include <vector>
#include <math.h>
#include <Vector3.h>

/// <summary>
/// 4x4行列
/// </summary>


struct Matrix4x4 final {

	inline static Matrix4x4 multiply(const Matrix4x4& mat1, const Matrix4x4& mat2) {
		// 結果の行列を初期化
		Matrix4x4 mutResult = {};

		// 行列の積を計算する
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				for (int k = 0; k < 4; ++k) {
					mutResult.m[i][j] += mat1.m[i][k] * mat2.m[k][j];
				}
			}
		}

		return mutResult;
	}

	float m[4][4];
	inline static Matrix4x4 MakeRotateXMatrix(float radian) {
		float cosTheta = std::cos(radian);
		float sinTheta = std::sin(radian);
		return {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cosTheta, sinTheta, 0.0f, 0.0f, -sinTheta, cosTheta, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
	}

	inline static Matrix4x4 MakeRotateYMatrix(float radian) {
		float cosTheta = std::cos(radian);
		float sinTheta = std::sin(radian);
		return { 
			cosTheta, 0.0f, -sinTheta, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			sinTheta, 0.0f, cosTheta, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}
	inline static Matrix4x4 MakeRotateZMatrix(float radian) {
			float cosTheta = std::cos(radian);
			float sinTheta = std::sin(radian);
		    return {cosTheta, sinTheta, 0.0f, 0.0f, -sinTheta, cosTheta, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
	}

	inline static Matrix4x4 Translate(float Tx, float Ty, float Tz) { 
		return {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 
			0.0f, 0.0f, 1.0f, 0.0f,
			Tx, Ty, Tz, 1.0f
		};
	}

	inline static Matrix4x4 Scale(float Sx, float Sy, float Sz) { 
		return {
			Sx, 0.0f, 0.0f, 0.0f,
			0.0f, Sy, 0.0f, 0.0f,
			0.0f, 0.0f, Sz, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}
		

	inline static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) { 
		Matrix4x4 rotateMatrix = multiply(MakeRotateXMatrix(rotate.x), MakeRotateYMatrix(rotate.y));
		rotateMatrix = multiply(rotateMatrix, MakeRotateZMatrix(rotate.z));

		Matrix4x4 resultMatrix2 = multiply(Scale(scale.x,scale.y,scale.z), rotateMatrix);
		resultMatrix2 = multiply(resultMatrix2, Translate(translate.x,translate.y,translate.z));
		
		return resultMatrix2;
	}

};

