#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"

class Skydome {

private:
	WorldTransform worldTransform_;

	ViewProjection* viewProjection_ = nullptr;

	Model* model_ = nullptr;

	// 22から

public:
	void Initialize(Model* model, ViewProjection* viewProjection);

	void Update();

	void Draw();
};
