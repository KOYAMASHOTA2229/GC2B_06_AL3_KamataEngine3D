#define NOMINMAX
#include "CameraController.h"
#include "Player.h"
#include <algorithm>

Vector3 Lerp(const Vector3& a, const Vector3& b, float t) { return {t * a.x + (1.0f - t) * b.x, t * a.y + (1.0f - t) * b.y, t * a.z + (1.0f - t) * b.z}; }

CameraController::CameraController() {}

CameraController::~CameraController() {}

void CameraController::Initialize() {
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void CameraController::Reset() {

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	viewProjection_.translation_ = targetWorldTransform.translation_ + targetOffset_;

}

void CameraController::Update() {


	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	
	Vector3 targetVelocity = target_->GetVelocity();
	targetPosition_ = targetWorldTransform.translation_ + targetOffset_ + targetVelocity * kVelocityBias_;

	viewProjection_.translation_ = Lerp(viewProjection_.translation_, targetPosition_, kIntrepolationRate);

	float positionX = targetWorldTransform.translation_.x;
	float positionY = targetWorldTransform.translation_.y;

	float viewProjectionX = viewProjection_.translation_.x;
	float viewProjectionY = viewProjection_.translation_.y;

	viewProjection_.translation_.x = std::clamp(viewProjectionX, positionX + margin_.left, positionX + margin_.right);
	viewProjection_.translation_.y = std::clamp(viewProjectionY, positionY + margin_.bottom, positionY + margin_.top);

	viewProjection_.translation_.x = std::clamp(viewProjectionX, movableArea_.left, movableArea_.right);
	viewProjection_.translation_.y = std::clamp(viewProjectionY, movableArea_.bottom, movableArea_.top);

	viewProjection_.UpdateMatrix();
}