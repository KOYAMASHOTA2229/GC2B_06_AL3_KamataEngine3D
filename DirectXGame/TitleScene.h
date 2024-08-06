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

~TitleScene();

void Initialize();

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
	uint32_t textureHandle_ = 0;


// 終了フラグ
bool isFinished_ = false;
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	Sprite* sprite_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;


};
