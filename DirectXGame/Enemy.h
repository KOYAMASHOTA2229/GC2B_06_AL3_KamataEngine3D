#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "structAABB.h"


#pragma once

//前方宣言
class Player;

class Enemy {

public:
	void Initialize(Model* model, ViewProjection* viewprojection, const Vector3& position);

	float Lerp(float radian);

	void Update();

	void Draw(const ViewProjection& viewprojection);

	Vector3 GetWorldPosition();

	AABB GetAABB();

	void OnCollision(Player* player);

private:
	Model* model_;

	WorldTransform worldTransform_;

	ViewProjection* viewProjection_;

	static inline const float kWalkSpeed = 0.05f;

	Vector3 velocity_ = {};

	// 首振り
	// 最初の角度
	static inline const float kWalkMotionAngleStart = 0.0f;
	// 最後の角度
	static inline const float kWalkMotionAngleEnd = 20.0f;
	// アニメーションの周期となる時間
	static inline const float kWalkMotionTime = 3.0f;

	float walkTimer_ = 0.0f;

	static inline const float kWidth = 2.0f;
	static inline const float kHeight = 2.0f;

};