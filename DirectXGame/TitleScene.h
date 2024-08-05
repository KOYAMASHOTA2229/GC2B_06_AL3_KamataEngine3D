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

void Initialize();

void Update();

void Draw();

/// デスフラグのgetter
bool GetIsFinished() const { return this->isFinished_; }

private:

// 終了フラグ
bool isFinished_ = false;
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;


};
