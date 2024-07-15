#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "GameScene.h"

class Skydome {

	private:
	WorldTransform worldTransform_;

	ViewProjection* viewProjection_;

	Model* model_ = nullptr;

	//22から

	public:
	void Initialize();

	void Update();

	void Draw();

};
