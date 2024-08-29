#pragma once
#include "WorldTransform.h"
#include "structAABB.h"

class Model;
class ViewProjection;
class Player;

class Clear {
private:
	Model* model_ = nullptr;

	ViewProjection* viewProjection_ = nullptr;

	WorldTransform worldTransform_;

	static inline const float kwidth = 1.8f;

	static inline const float kheight = 1.8f;

	public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

	void OnCollision(Player* player);

	AABB GetAABB();

	Vector3 GetWorldPosition();

};
