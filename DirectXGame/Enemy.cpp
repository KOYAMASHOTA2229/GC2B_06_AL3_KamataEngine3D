#include "Enemy.h"
#include <cassert>
#include <numbers>
#define _USE_MATH_DEFINES
#include <cmath>



void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {

	assert(model);

	model_ = model;

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	worldTransform_.rotation_.y = -std::numbers::pi_v<float> / 2.0f;

	viewProjection_ = viewProjection;

	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
}

float Enemy::Lerp(float radian) { return radian * (3.14f / 180.0f); }

void Enemy::Update() {

	worldTransform_.translation_.x += velocity_.x;

	// タイマー
	walkTimer_ += 1.0f / 60.0f;

	float Pi = 3.14f;

	float moveX = (2 * Pi) * walkTimer_ / kWalkMotionTime;

	// 回転アニメーション
	float param = std::sin(moveX);
	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = Enemy::Lerp(radian);

	// これで上記を更新するので一番下
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewprojection) { model_->Draw(worldTransform_, viewprojection); }

Vector3 Enemy::GetWorldPosition() { 

	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;

}

AABB Enemy::GetAABB() { 

	Vector3 worldPos = Enemy::GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;

}
