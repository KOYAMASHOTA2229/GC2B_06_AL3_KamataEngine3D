#include "Clear.h"
#include "Model.h"
#include "ViewProjection.h"
#include "cassert"

void Clear::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {

	assert(model);

	model_ = model;

	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

}

void Clear::Update() { worldTransform_.UpdateMatrix(); }

void Clear::Draw() { model_->Draw(worldTransform_, *viewProjection_); }

void Clear::OnCollision(Player* player) {

	(void)player;

	worldTransform_.rotation_ += {0, 0.5f, 0};
}

AABB Clear::GetAABB() {

	Vector3 worldPos = Clear::GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kwidth / 2.0f, worldPos.y - kheight / 2.0f, worldPos.z - kwidth / 2.0f};
	aabb.max = {worldPos.x + kwidth / 2.0f, worldPos.y + kheight / 2.0f, worldPos.z + kwidth / 2.0f};

	return aabb;
}

Vector3 Clear::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}