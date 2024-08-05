#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class TitleScene {

	public:

void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection);

void Update();

void Draw();

/// デスフラグのgetter
bool GetIsFinished() const { return this->isFinished_; }

private:

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;

// 終了フラグ
bool isFinished_ = false;

};
